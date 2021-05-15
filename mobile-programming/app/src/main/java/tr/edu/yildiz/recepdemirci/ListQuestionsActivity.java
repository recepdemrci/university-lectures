package tr.edu.yildiz.recepdemirci;

import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Intent;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.PopupWindow;
import android.widget.Toast;

import java.util.ArrayList;

public class ListQuestionsActivity extends AppCompatActivity implements ClickListener {
    QuestionDatabase exam;
    QuestionDatabase questionDatabase;
    ArrayList<Question> questions;

    RecyclerView questionsRecView;
    QuestionsRecViewAdapter adapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list_questions);

        // Initialize variables
        exam = new QuestionDatabase(getApplicationContext(), getIntent().getExtras().getString("userMail"),true);
        questionDatabase = new QuestionDatabase(getApplicationContext(), getIntent().getExtras().getString("userMail"));
        questions = questionDatabase.getQuestions();

        // Initialize view
        questionsRecView = findViewById(R.id.questions_recycler_view);
        adapter = new QuestionsRecViewAdapter(this, getApplicationContext(), R.layout.questions_list_item);
        adapter.setQuestions(questions);
        questionsRecView.setAdapter(adapter);
        questionsRecView.setLayoutManager(new LinearLayoutManager(this, LinearLayoutManager.HORIZONTAL, false));

        try {
            int position = getIntent().getExtras().getInt("position");
            questionsRecView.scrollToPosition(position);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onSelectClick(View view, int position) {
        if (exam.add(getApplicationContext(), questions.get(position))) {
            Toast.makeText(getApplicationContext(),
                    "Question add to exam",Toast.LENGTH_SHORT).show();
        }
        else if(exam.getQuestions().contains(questions.get(position))) {
            Toast.makeText(getApplicationContext(),
                    "Error! Question is already in exam",Toast.LENGTH_SHORT).show();
        }
        else {
            Toast.makeText(getApplicationContext(),
                    "Error! Question cold not add to exam",Toast.LENGTH_SHORT).show();
        }
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
                if (questionDatabase.remove(getApplicationContext(), questions.get(position))){
                    popupWindow.dismiss();
                    adapter.notifyDataSetChanged();
                    Toast.makeText(getApplicationContext(),
                            "Question deleted",Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(getApplicationContext(),
                            "Error! Question could not deleted",Toast.LENGTH_SHORT).show();
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
        Intent intent = new Intent(this, EditQuestionActivity.class);
        intent.putExtra("position", position);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

    @Override
    public void onSaveClick(View view, int position) {
        Intent intent = new Intent(this, CreateExamActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

    @Override
    public void onBackPressed() {
        Intent intent = new Intent(this, MenuActivity.class);
        intent.putExtra("userMail", getIntent().getExtras().getString("userMail"));
        startActivity(intent);
    }

}