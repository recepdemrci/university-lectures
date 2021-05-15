package tr.edu.yildiz.recepdemirci;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.cardview.widget.CardView;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;


public class QuestionsRecViewAdapter extends RecyclerView.Adapter<QuestionsRecViewAdapter.ViewHolder> {
    final int layout;
    final ClickListener listener;
    ArrayList<Question> questions = new ArrayList<>();

    SharedPreferences sharedPreferences;


    public QuestionsRecViewAdapter(ClickListener listener, Context context, int layout) {
        this.sharedPreferences = context.getSharedPreferences("exam_settings", Context.MODE_PRIVATE);
        // Listeners for buttons
        this.listener = listener;
        // For use same adapter with different layout (ListQuestion & Exam)
        this.layout = layout;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(layout, parent, false);
        ViewHolder holder = new ViewHolder(view);
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        Question question = questions.get(position);
        ArrayList<String> answers = question.getAnswers(sharedPreferences.getInt("level", 5));
        Bitmap image = question.getImage();

        if (image != null) {
            holder.image_question.setImageBitmap(question.getImage());
        }
        holder.text_question.setText(question.getQuestion(position+1));
        holder.text_answer1.setText(answers.get(0));
        holder.text_answer2.setText(answers.get(1));
        holder.text_answer3.setText(answers.get(2));
        holder.text_answer4.setText(answers.get(3));
        holder.text_answer5.setText(answers.get(4));
        holder.text_correct_answer.setText(question.getCorrect_answer());
    }

    @Override
    public int getItemCount() {
        return questions.size();
    }

    public void setQuestions(ArrayList<Question> questions) {
        this.questions = questions;
        notifyDataSetChanged();
    }


    public class ViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
       CardView button_edit_send;
       CardView button_delete;
       CardView button_select;
       CardView button_save;
       ImageView image_question;
       TextView text_question;
       TextView text_answer1;
       TextView text_answer2;
       TextView text_answer3;
       TextView text_answer4;
       TextView text_answer5;
       TextView text_correct_answer;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            image_question = itemView.findViewById(R.id.image_question);
            text_question = itemView.findViewById(R.id.text_question);
            text_answer1 = itemView.findViewById(R.id.text_answer1);
            text_answer2 = itemView.findViewById(R.id.text_answer2);
            text_answer3 = itemView.findViewById(R.id.text_answer3);
            text_answer4 = itemView.findViewById(R.id.text_answer4);
            text_answer5 = itemView.findViewById(R.id.text_answer5);
            text_correct_answer = itemView.findViewById(R.id.correct_answer);

            button_edit_send = itemView.findViewById(R.id.edit_send);
            button_delete = itemView.findViewById(R.id.delete);
            button_select = itemView.findViewById(R.id.select_question);
            button_save = itemView.findViewById(R.id.save);
            itemView.setOnClickListener(this);
            button_edit_send.setOnClickListener(this);
            button_delete.setOnClickListener(this);
            button_select.setOnClickListener(this);
            button_save.setOnClickListener(this);
        }

        @Override
        public void onClick(View v) {
            int position = getAdapterPosition();
            if (v.getId() == button_edit_send.getId()) {
                listener.onEditorSendClick(v, position);
            }
            else if (v.getId() == button_delete.getId()) {
                listener.onDeleteClick(v, position);
            }
            else if (v.getId() == button_select.getId()) {
                listener.onSelectClick(v, position);
            }
            else if (v.getId() == button_save.getId()) {
                listener.onSaveClick(v, position);
            }
        }

    }
}
