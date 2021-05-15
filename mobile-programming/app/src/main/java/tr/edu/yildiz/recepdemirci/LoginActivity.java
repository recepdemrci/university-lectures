package tr.edu.yildiz.recepdemirci;

import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;


public class LoginActivity extends AppCompatActivity {
    int counter;
    UserDatabase userDatabase;

    EditText email;
    EditText password;
    CardView buttonLogin;
    CardView buttonSignup;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        // Initialize variables
        counter = 0;
        userDatabase = new UserDatabase(getApplicationContext());

        // Initialize view
        email = (EditText) findViewById(R.id.email);
        password = (EditText) findViewById(R.id.password);
        buttonLogin = (CardView) findViewById(R.id.login);
        buttonSignup = (CardView) findViewById(R.id.signup);
    }

    public void onLoginClick(View view) {
        User tempUser = new User(email.getText().toString(), password.getText().toString());
        if(userDatabase.getUsers().contains(tempUser)){
            Intent intent = new Intent(this, MenuActivity.class);
            intent.putExtra("userMail", email.getText().toString());
            startActivity(intent);
            Toast.makeText(getApplicationContext(),
                    "Redirecting...",Toast.LENGTH_SHORT).show();
        }
        else{
            counter++;
            if(counter >= 3) {
                buttonLogin.setEnabled(false);
                Intent intent = new Intent(this, SignupActivity.class);
                startActivity(intent);
                Toast.makeText(getApplicationContext(),
                        "3 Wrong Attempt. Redirecting to Sign Up Page",Toast.LENGTH_SHORT).show();
                overridePendingTransition(android.R.anim.slide_out_right, android.R.anim.slide_in_left);
            }
            Toast.makeText(getApplicationContext(),
                    "Wrong Credentials",Toast.LENGTH_SHORT).show();
        }
    }

    public void onSignupClick(View view) {
        Intent intent = new Intent(this, SignupActivity.class);
        startActivity(intent);
        overridePendingTransition(android.R.anim.slide_out_right, android.R.anim.slide_in_left);
    }

}