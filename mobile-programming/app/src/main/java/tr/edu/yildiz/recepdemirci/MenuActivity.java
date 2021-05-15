package tr.edu.yildiz.recepdemirci;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

public class MenuActivity extends AppCompatActivity {
    User user;
    UserDatabase userDatabase;

    ImageView imageView;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        // Initialize variables
        userDatabase = new UserDatabase(getApplicationContext());
        user = userDatabase.getUser(getIntent().getExtras().getString("userMail"));

        // Initialize view
        imageView = findViewById(R.id.imageView);
        if (user.getImage() != null) {
            imageView.setImageBitmap(user.getImage());
        }
    }

    public void onNewQuestionClick(View view) {
        Intent intent = new Intent(this, NewQuestionActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

    public void onListQuestionsClick(View view) {
        Intent intent = new Intent(this, ListQuestionsActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

    public void onCreateExamClick(View view) {
        Intent intent = new Intent(this, CreateExamActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

    public void onExamSettingsClick(View view) {
        Intent intent = new Intent(this, ExamSettingsActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

    public void onSignoutClick(View view) {
        Intent intent = new Intent(this, LoginActivity.class);
        startActivity(intent);
    }
}