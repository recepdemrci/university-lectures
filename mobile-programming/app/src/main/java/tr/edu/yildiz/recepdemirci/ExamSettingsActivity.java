package tr.edu.yildiz.recepdemirci;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Toast;

public class ExamSettingsActivity extends AppCompatActivity {
    int time;
    int level;
    float score;

    SharedPreferences sharedPreferences;
    SeekBar seekTime;
    SeekBar seekLevel;
    EditText textScore;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_exam_settings);

        // Initialize Shared Preferences
        sharedPreferences = getSharedPreferences("exam_settings", MODE_PRIVATE);
        time = sharedPreferences.getInt("time", 0);
        level = sharedPreferences.getInt("level", 2);
        score = sharedPreferences.getFloat("score", 10);

        // Initialize view
        seekTime = findViewById(R.id.seekTime);
        seekLevel = findViewById(R.id.seekLevel);
        textScore = findViewById(R.id.score);
        seekTime.setMax(180);
        seekLevel.setMax(3);
        seekTime.setProgress(time);
        seekLevel.setProgress(level-2);
        textScore.setText(String.format("%s", score));

        // Seekbar Listeners
        seekTime.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                time = progress;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Toast.makeText(getApplicationContext(),
                        "Exam Time: " + time + "min.", Toast.LENGTH_SHORT).show();
            }
        });

        seekLevel.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                level = progress + 2;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Toast.makeText(getApplicationContext(),
                        "Exam Level: " + level, Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void onSaveClick(View view) {
        try {
            score = Float.parseFloat(textScore.getText().toString());
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (score <= 0) {
            score = 10;
        }

        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putFloat("score", score);
        editor.putInt("time", time);
        editor.putInt("level", level);
        editor.commit();

        Intent intent = new Intent(this, MenuActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }
}