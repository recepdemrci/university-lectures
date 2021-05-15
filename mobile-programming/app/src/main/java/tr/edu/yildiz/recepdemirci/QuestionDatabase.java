package tr.edu.yildiz.recepdemirci;

import android.content.Context;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

public class QuestionDatabase {
    private final String filepath;
    private String filepathTxt;
    private ArrayList<Question> questions = new ArrayList<>();


    public QuestionDatabase(Context context, String userMail) {
        filepath = userMail + "-Questions";
        readFromFile(context);
    }

    public QuestionDatabase(Context context, String userMail, boolean isExam) {
        filepath = userMail + "-Exam";
        filepathTxt = userMail + "-Exam.txt";
        readFromFile(context);
    }

    public String getFilepathTxt() {
        return filepathTxt;
    }

    public ArrayList<Question> getQuestions() {
        return questions;
    }

    public boolean add(Context context, Question question) {
        if (questions.contains(question)) {
            return false;
        }
        else {
            questions.add(question);
            return writeToFile(context);
        }
    }

    public boolean remove(Context context, Question question) {
        if (questions.contains(question)) {
            questions.remove(question);
            return writeToFile(context);
        }
        else {
            return false;
        }
    }

    public boolean edit(Context context, Question question, int position) {
        // control if edited question is same with another question
        if (questions.contains(question) && questions.indexOf(question) != position){
            return false;
        }
        else {
            questions.set(position, question);
            return writeToFile(context);
        }
    }

    private void deleteAll(Context context) {
        questions = new ArrayList<Question>();
        writeToFile(context);
    }

    private void readFromFile(Context context) {
        try {
            FileInputStream fileIn = context.openFileInput(filepath);
            ObjectInputStream objectIn = new ObjectInputStream(fileIn);
            questions = (ArrayList<Question>) objectIn.readObject();
            objectIn.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private boolean writeToFile(Context context) {
        try {
            FileOutputStream fileOut = context.openFileOutput(filepath, Context.MODE_PRIVATE);
            ObjectOutputStream objectOut = new ObjectOutputStream(fileOut);
            objectOut.writeObject(questions);
            objectOut.close();
            return true;
        } catch (Exception ex) {
            return false;
        }
    }

    public boolean saveToTxt(Context context, int level) {
        try {
            File file = new File(context.getFilesDir(), filepathTxt);
            FileWriter writer = new FileWriter(file);
            int index = 1;
            for (Question question_ : questions) {
                writer.append(String.valueOf(index)).append(question_.toString(level));
                writer.flush();
                index ++;
            }
            writer.close();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public String loadFromTxt(Context context) {
        File file = new File(context.getFilesDir(), filepathTxt);
        StringBuilder text = new StringBuilder();
        try {
            BufferedReader br = new BufferedReader(new FileReader(file));
            String line;
            while ((line = br.readLine()) != null) {
                text.append(line);
                text.append('\n');
            }
            br.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return text.toString();
    }

}
