package tr.edu.yildiz.recepdemirci;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.Toast;
import android.widget.VideoView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

public class EditQuestionActivity extends AppCompatActivity {
    private static final int GALLERY_REQUEST = 100;
    private static final int GALLERY_VIDEO_REQUEST = 200;

    int position;
    int correct_answer;
    String image_path;
    String video_path;
    QuestionDatabase questionDatabase;

    EditText question;
    EditText answer_1;
    EditText answer_2;
    EditText answer_3;
    EditText answer_4;
    EditText answer_5;
    ImageView imageView;
    VideoView videoView;
    CardView buttonSave;
    CardView buttonAddImage;
    CardView buttonCancel;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_question);

        // Initialize variables
        correct_answer = -1;
        questionDatabase = new QuestionDatabase(getApplicationContext(),
                getIntent().getExtras().getString("userMail"));
        position = getIntent().getExtras().getInt("position");

        // Initialize view
        question = (EditText) findViewById(R.id.text_question);
        answer_1 = (EditText) findViewById(R.id.text_answer1);
        answer_2 = (EditText) findViewById(R.id.text_answer2);
        answer_3 = (EditText) findViewById(R.id.text_answer3);
        answer_4 = (EditText) findViewById(R.id.text_answer4);
        answer_5 = (EditText) findViewById(R.id.text_answer5);
        imageView = (ImageView) findViewById(R.id.imageView);
        videoView = (VideoView) findViewById(R.id.videoView);
        buttonSave = (CardView) findViewById(R.id.save);
        buttonAddImage = (CardView) findViewById(R.id.add_image);
        buttonCancel = (CardView) findViewById(R.id.button_cancel);
        initialize();
    }


    public void initialize() {
        Question q = questionDatabase.getQuestions().get(position);
        ArrayList<String> answers = (ArrayList<String>) q.getAnswers();

        question.setText(q.getQuestion());
        answer_1.setText(answers.get(0));
        answer_2.setText(answers.get(1));
        answer_3.setText(answers.get(2));
        answer_4.setText(answers.get(3));
        answer_5.setText(answers.get(4));
        imageView.setImageBitmap(q.getImage());
        videoView.setVideoURI(q.getVideo());
        videoView.requestFocus();
        videoView.start();
        image_path = q.getImage_path();
        video_path = q.getVideo_path();
    }

    public void onSaveClick(View view) {
        if (controlInput()){
            ArrayList<String> answers = new ArrayList<>();
            answers.add(answer_1.getText().toString());
            answers.add(answer_2.getText().toString());
            answers.add(answer_3.getText().toString());
            answers.add(answer_4.getText().toString());
            answers.add(answer_5.getText().toString());

            Question question_ = new Question(
                    question.getText().toString(),
                    answers,
                    correct_answer,
                    image_path,
                    video_path);

            if (questionDatabase.edit(getApplicationContext(), question_, position)){
                Toast.makeText(getApplicationContext(), "Question saved", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(this, ListQuestionsActivity.class);
                intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
                intent.putExtra("position", position);
                startActivity(intent);
            }
            else {
                Toast.makeText(getApplicationContext(),
                        "Error! Question is already exist",Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void onAddImageClick(View view) {
        Intent intent = new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent, "Select Image"), GALLERY_REQUEST);
    }

    public void onAddVideoClick(View view) {
        Intent intent = new Intent();
        intent.setType("video/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent, "Select Video"), GALLERY_VIDEO_REQUEST);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == RESULT_OK && data != null) {
            switch (requestCode) {
                case GALLERY_REQUEST:
                    try {
                        Uri selectedImage = data.getData();
                        InputStream imageStream = getContentResolver().openInputStream(selectedImage);
                        imageView.setImageBitmap(BitmapFactory.decodeStream(imageStream));

                        Bitmap image = ((BitmapDrawable)imageView.getDrawable()).getBitmap();
                        saveImage(image, selectedImage.getPath().substring(selectedImage.getPath().lastIndexOf( '/')+1));
                    } catch (IOException exception) {
                        exception.printStackTrace();
                    }
                    break;
                case GALLERY_VIDEO_REQUEST:
                    Uri selectedVideo = data.getData();
                    videoView.setVideoURI(selectedVideo);
                    videoView.requestFocus();
                    videoView.start();
                    video_path = selectedVideo.getPath();
                    Toast.makeText(getApplicationContext(), "Video added", Toast.LENGTH_SHORT).show();
            }
        }
    }

    public void onCancelClick(View view) {
        Intent intent = new Intent(this, ListQuestionsActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        intent.putExtra("position", position);
        startActivity(intent);
    }

    @Override
    public void onBackPressed() {
        Intent intent = new Intent(this, ListQuestionsActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        intent.putExtra("position", position);
        startActivity(intent);
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
                Toast.makeText(getApplicationContext(), "Image added", Toast.LENGTH_SHORT).show();
            } catch (Exception e) {
                Toast.makeText(getApplicationContext(), "Error! Image couldn't added", Toast.LENGTH_SHORT).show();
                e.printStackTrace();
            }
        }else{
            Toast.makeText(this, "Failed to make folder!", Toast.LENGTH_SHORT).show();
        }
    }

    private boolean controlInput() {
        if (question.getText().toString().isEmpty() ||
                answer_1.getText().toString().isEmpty() ||
                answer_2.getText().toString().isEmpty() ||
                answer_3.getText().toString().isEmpty() ||
                answer_4.getText().toString().isEmpty() ||
                answer_5.getText().toString().isEmpty()) {
            Toast.makeText(getApplicationContext(), "Error! Enter Question and all the answers", Toast.LENGTH_SHORT).show();
            return false;
        }
        if (correct_answer == -1) {
            Toast.makeText(getApplicationContext(), "Error! Correct answer did not chosen", Toast.LENGTH_SHORT).show();
            return false;
        }
        return true;
    }


    @SuppressLint("NonConstantResourceId")
    public void onRadioButtonClicked(View view) {
        boolean checked = ((RadioButton) view).isChecked();

        switch(view.getId()) {
            case R.id.checkBoxA:
                if (checked)
                    correct_answer = 0;
                break;
            case R.id.checkBoxB:
                if (checked)
                    correct_answer = 1;
                break;
            case R.id.checkBoxC:
                if (checked)
                    correct_answer = 2;
                break;
            case R.id.checkBoxD:
                if (checked)
                    correct_answer = 3;
                break;
            case R.id.checkBoxE:
                if (checked)
                    correct_answer = 4;
                break;
        }
    }
}