package tr.edu.yildiz.recepdemirci;

import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.PopupWindow;
import android.widget.SeekBar;
import android.widget.Toast;

import java.util.ArrayList;

public class CreateExamActivity extends AppCompatActivity implements ClickListener {
    int time;
    int level;
    float score;
    QuestionDatabase exam;
    ArrayList<Question> questions;

    SharedPreferences sharedPreferences;
    QuestionsRecViewAdapter adapter;
    RecyclerView questionsRecView;
    SeekBar seekTime;
    SeekBar seekLevel;
    EditText textScore;
    CardView button_ok;
    CardView button_select;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_exam);

        // Initialize Shared Preferences
        sharedPreferences = getSharedPreferences("exam_settings", MODE_PRIVATE);
        time = sharedPreferences.getInt("time", 0);
        level = sharedPreferences.getInt("level", 2);
        score = sharedPreferences.getFloat("score", 10);

        // Initialize variables
        exam = new QuestionDatabase(getApplicationContext(), getIntent().getExtras().getString("userMail"),true);
        questions = exam.getQuestions();

        // Initialize view
        button_select = findViewById(R.id.select_question);
        questionsRecView = findViewById(R.id.exam_questions_recycler_view);
        adapter = new QuestionsRecViewAdapter(this, getApplicationContext(), R.layout.exam_questions_list_item);
        adapter.setQuestions(questions);
        questionsRecView.setAdapter(adapter);
        questionsRecView.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false));

        // Control if no question in exam yet, and make visible select button
        if (questions.isEmpty()) {
            button_select.setVisibility(View.VISIBLE);
            questionsRecView.setVisibility(View.GONE);
        }
        else {
            button_select.setVisibility(View.GONE);
            questionsRecView.setVisibility(View.VISIBLE);
        }
    }

    public void onExamSettingsClick(View view) {
        LayoutInflater layoutInflater = (LayoutInflater)getBaseContext().getSystemService(LAYOUT_INFLATER_SERVICE);
        View popupView = layoutInflater.inflate(R.layout.popup_settings, null);
        final PopupWindow popupWindow = new PopupWindow(
                popupView,
                DrawerLayout.LayoutParams.WRAP_CONTENT,
                DrawerLayout.LayoutParams.WRAP_CONTENT);

        button_ok = popupView.findViewById(R.id.ok);
        seekTime = popupView.findViewById(R.id.seekTime);
        seekLevel = popupView.findViewById(R.id.seekLevel);
        textScore = popupView.findViewById(R.id.score);
        seekTime.setMax(180);
        seekLevel.setMax(3);
        seekTime.setProgress(time);
        seekLevel.setProgress(level-2);
        textScore.setText(String.format("%s", score));
        popupWindow.showAtLocation(view, Gravity.CENTER, 0, 0);

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

        button_ok.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                SharedPreferences.Editor editor = sharedPreferences.edit();
                try {
                    score = Float.parseFloat(textScore.getText().toString());
                } catch (Exception e) {
                    e.printStackTrace();
                }
                if (score <= 0) {
                    score = 10;
                }
                popupWindow.dismiss();
                editor.putInt("time", time);
                editor.putInt("level", level);
                editor.putFloat("score", score);
                editor.apply();
                adapter.notifyDataSetChanged();
                Toast.makeText(getApplicationContext(),
                        "Exam settings saved",Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    public void onSelectClick(View view, int position) {
        Toast.makeText(getApplicationContext(),
                "Redirecting to list of questions",Toast.LENGTH_SHORT).show();
        Intent intent = new Intent(this, ListQuestionsActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

    public void onSelectClick(View view) {
        Toast.makeText(getApplicationContext(),
                "Redirecting to list of questions",Toast.LENGTH_SHORT).show();
        Intent intent = new Intent(this, ListQuestionsActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }


    @Override
    public void onDeleteClick(View view, int position) {
        LayoutInflater layoutInflater = (LayoutInflater)getBaseContext().getSystemService(LAYOUT_INFLATER_SERVICE);
        View popupView = layoutInflater.inflate(R.layout.popup, null);
        final PopupWindow popupWindow = new PopupWindow(
                popupView,
                DrawerLayout.LayoutParams.WRAP_CONTENT,
                DrawerLayout.LayoutParams.WRAP_CONTENT);
        CardView button_delete_ = popupView.findViewById(R.id.delete_);
        CardView button_cancel_ = popupView.findViewById(R.id.cancel_);
        popupWindow.showAtLocation(view, Gravity.CENTER, 0, 0);

        button_delete_.setOnClickListener(new CardView.OnClickListener(){
            @Override
            public void onClick(View v){
                if (exam.remove(getApplicationContext(), questions.get(position))){
                    popupWindow.dismiss();
                    adapter.notifyDataSetChanged();
                    Toast.makeText(getApplicationContext(),
                            "Question removed from the exam",Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(getApplicationContext(),
                            "Error! Question could not removed",Toast.LENGTH_SHORT).show();
                }
            }
        });

        button_cancel_.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                popupWindow.dismiss();
            }
        });
    }

    @Override
    public void onEditorSendClick(View view, int position) {
        exam.saveToTxt(getApplicationContext(), level);
        String text = exam.loadFromTxt(getApplicationContext());

        Intent intent = new Intent();
        intent.setAction(Intent.ACTION_SEND);
        intent.setType("text/plain");
        intent.putExtra(Intent.EXTRA_TEXT, text);
        if (intent.resolveActivity(getPackageManager()) == null) {
            Toast.makeText(this,"Error! No compatible application", Toast.LENGTH_SHORT).show();
            return;
        }
        startActivity(intent);
    }

    @Override
    public void onSaveClick(View view, int position) {
        if(exam.saveToTxt(getApplicationContext(), level)) {
            Toast.makeText(getApplicationContext(),
                    "Exam saved",Toast.LENGTH_SHORT).show();
        }
        else {
            Toast.makeText(getApplicationContext(),
                    "Error! Exam could not saved",Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onBackPressed() {
        Intent intent = new Intent(this, MenuActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

}