package tr.edu.yildiz.recepdemirci;

import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;

import android.app.DatePickerDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.time.LocalDate;
import java.util.Calendar;

public class SignupActivity extends AppCompatActivity {
    private static final int GALLERY_REQUEST = 100;

    String image_path;
    LocalDate birthdate;
    UserDatabase userDatabase;

    EditText name;
    EditText surname;
    EditText textBirthdate;
    EditText email;
    EditText phone;
    EditText password;
    EditText confirm;
    ImageView imageView;
    CardView buttonSignup;
    CardView buttonCancel;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sigup);

        // Initialize variables
        userDatabase = new UserDatabase(getApplicationContext());

        // Initialize view
        name = findViewById(R.id.name);
        surname = findViewById(R.id.surname);
        textBirthdate = findViewById(R.id.birthdate);
        email = findViewById(R.id.email);
        phone = findViewById(R.id.phone);
        password = findViewById(R.id.password);
        confirm = findViewById(R.id.confirm);
        imageView = findViewById(R.id.imageView);
        buttonSignup = findViewById(R.id.signup);
        buttonCancel = findViewById(R.id.button_cancel);
    }

    public void onSignupClick(View view) {
        if (controlInputs()) {
            User user = new User(
                        name.getText().toString(),
                        surname.getText().toString(),
                        email.getText().toString(),
                        phone.getText().toString(),
                        password.getText().toString(),
                        birthdate,
                        image_path);

            if (userDatabase.getUsers().contains(user)) {
                Toast.makeText(getApplicationContext(),
                        "Error! User is already exist with same email",Toast.LENGTH_SHORT).show();
                return;
            }

            if (userDatabase.add(getApplicationContext(), user)) {
                Intent intent = new Intent(this, LoginActivity.class);
                intent.putExtra("userMail", email.getText().toString());
                startActivity(intent);
                Toast.makeText(getApplicationContext(),
                        "New user added",Toast.LENGTH_SHORT).show();
                overridePendingTransition(android.R.anim.slide_in_left, android.R.anim.slide_out_right);
            }
            else {
                Toast.makeText(getApplicationContext(),
                        "Error! Try again",Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void onCancelClick(View view) {
        Intent intent= new Intent(this, LoginActivity.class);
        startActivity(intent);
        overridePendingTransition(android.R.anim.slide_in_left, android.R.anim.slide_out_right);
    }

    public void onAddImageClick(View view) {
        Intent intent = new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent, "Select Picture"), GALLERY_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == GALLERY_REQUEST && resultCode == RESULT_OK && null != data) {
            try {
                Uri selectedImage = data.getData();
                InputStream imageStream = getContentResolver().openInputStream(selectedImage);
                Bitmap image = BitmapFactory.decodeStream(imageStream);
                saveImage(image, selectedImage.getPath().substring(selectedImage.getPath().lastIndexOf( '/')+1));

                imageView.setImageBitmap(image);
            } catch (IOException exception) {
                exception.printStackTrace();
            }
        }
    }

    private void saveImage(Bitmap image, String imageFileName) {
        String dirPath = getFilesDir().getAbsolutePath() + "/images/";
        final File storageDir = new File(dirPath);

        boolean successDirCreated = true;
        if (!storageDir.exists()) {
            successDirCreated = storageDir.mkdir();
        }
        if (successDirCreated) {
            File imageFile = new File(storageDir, imageFileName);
            try {
                OutputStream fOut = new FileOutputStream(imageFile);
                image.compress(Bitmap.CompressFormat.JPEG, 100, fOut);
                fOut.close();
                image_path = imageFile.getAbsolutePath();
            } catch (Exception e) {
                Toast.makeText(getApplicationContext(), "Error! while adding image", Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
        }else{
            Toast.makeText(this, "Error! Failed to make folder", Toast.LENGTH_SHORT).show();
        }
    }

    public void onBirthdateClick(View view) {
        DatePickerDialog datePicker;
        final Calendar calendar = Calendar.getInstance();
        int day = calendar.get(Calendar.DAY_OF_MONTH);
        int month = calendar.get(Calendar.MONTH);
        int year = calendar.get(Calendar.YEAR);

        datePicker = new DatePickerDialog(SignupActivity.this,
                new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int monthOfYear, int dayOfMonth) {
                        birthdate = LocalDate.of(year, monthOfYear, dayOfMonth);
                        textBirthdate.setText(birthdate.toString());
                    }
                }, year, month, day);
        datePicker.show();
    }

    public boolean controlInputs(){
        String emailPattern = "[a-zA-Z0-9._-]+@[a-z]+\\.+[a-z]+";
        String phonePattern = "[\\+\\d]?(\\d{2,3}[-\\.\\s]??\\d{2,3}[-\\.\\s]??\\d{4}|\\(\\d{3}\\)\\s*\\d{3}[-\\.\\s]??\\d{4}|\\d{3}[-\\.\\s]??\\d{4})";

        if (!email.getText().toString().matches(emailPattern) || email.getText().toString().isEmpty()) {
            Toast.makeText(getApplicationContext(),
                    "Error! Invalid email",Toast.LENGTH_SHORT).show();
            return false;
        }
        if (name.getText().toString().isEmpty()) {
            Toast.makeText(getApplicationContext(),
                    "Error! Invalid name",Toast.LENGTH_SHORT).show();
            return false;
        }
        if (!phone.getText().toString().matches(phonePattern)){
            Toast.makeText(getApplicationContext(),
                    "Error! Invalid phone number",Toast.LENGTH_SHORT).show();
            return false;
        }
        if (password.getText().toString().isEmpty()) {
            Toast.makeText(getApplicationContext(),
                    "Error! Password is empty",Toast.LENGTH_SHORT).show();
            return false;
        }
        if (!password.getText().toString().equals(confirm.getText().toString())) {
            Toast.makeText(getApplicationContext(),
                    "Error! Passwords do not match!",Toast.LENGTH_SHORT).show();
            return false;
        }
        return true;
    }
}