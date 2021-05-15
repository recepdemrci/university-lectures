package tr.edu.yildiz.recepdemirci;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;

import java.io.FileInputStream;
import java.io.InputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Question implements Serializable {
    private String question;
    private ArrayList<String> answers;
    private int correct_answer;
    private int temp_correct_answer;
    private String image_path;
    private String video_path;


    public Question(String question, ArrayList<String> answers, int correct_answer, String image_path, String video_path) {
        this.question = question;
        this.answers = answers;
        this.correct_answer = correct_answer;
        this.image_path = image_path;
        this.video_path = video_path;
    }

    public String getQuestion() {
        return  question;
    }

    public String getQuestion(int q_number) {
        return q_number + " -  " + question;
    }

    public List<String> getAnswers() {
        return answers;
    }

    public ArrayList<String> getAnswers(int level) {
        ArrayList<String> answers_  = new ArrayList<>();
        boolean flag = false;
        int start = 'a';
        char choice = (char)start;

        for (int i=0; i < level; i++) {
            if (i == correct_answer) {
                temp_correct_answer = correct_answer;
                flag = true;
            }
            choice = (char) (start + i);
            answers_.add(choice + " - " + answers.get(i));
        }
        if (!flag) {
            Random random = new Random();
            temp_correct_answer = random.nextInt(level);
            choice = (char) (start + temp_correct_answer);
            answers_.set(temp_correct_answer, choice + " - " + answers.get(correct_answer));
        }

        for (int i=level; i < 5; i++) {
            answers_.add(null);
        }
        return answers_;
    }

    public String getCorrect_answer() {
        String result = "Correct Answer: ";
        switch (temp_correct_answer){
            case 0:
                return result + "A";
            case 1:
                return result + "B";
            case 2:
                return result + "C";
            case 3:
                return result + "D";
            case 4:
                return result + "E";
            default:
                return result + "-";
        }
    }

    public String getImage_path() {
        return image_path;
    }


    public String getVideo_path() {
        return video_path;
    }

    public Bitmap getImage() {
        try {
            InputStream fileIn = new FileInputStream(image_path);
            Bitmap image = BitmapFactory.decodeStream(fileIn);
            fileIn.close();
            return image;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public Uri getVideo() {
        try {
            return Uri. parse(video_path);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    @Override
    public boolean equals(Object o) {
        Question question = (Question) o;
        return question.question.equals(this.question) &&
                question.answers.equals(this.answers) &&
                question.correct_answer == this.correct_answer;
    }

    public String toString(int level) {
        ArrayList<String> answers_ = getAnswers(level);
        String result = " - " + question + '\n';

        for (String answer : answers_) {
            if (answer != null){
                result = result + answer + '\n';
            }
        }
        result += "\n\n";
        return result;
    }
}
