package tr.edu.yildiz.recepdemirci;


import android.content.Context;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

public class UserDatabase {
    private static final String filepath="Users";
    private ArrayList<User> users = new ArrayList<>();

    public UserDatabase(Context context) {
        readFromFile(context);
    }

    public ArrayList<User> getUsers() {
       return users;
    }

    public User getUser(String email) {
        for (User user:users) {
            if (email.equals(user.getEmail())){
                return user;
            }
        }
        return null;
    }

    public boolean add(Context context, User user) {
        if (users.contains(user)) {
            return false;
        }
        else {
            users.add(user);
            return writeToFile(context);
        }
    }

    public boolean remove(Context context, User user) {
        if (users.contains(user)) {
            users.remove(user);
            return writeToFile(context);
        }
        else {
            return false;
        }
    }

    private void readFromFile(Context context) {
        try {
            FileInputStream fileIn = context.openFileInput(filepath);
            ObjectInputStream objectIn = new ObjectInputStream(fileIn);
            users = (ArrayList<User>) objectIn.readObject();
            objectIn.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private boolean writeToFile(Context context) {
        try {
            FileOutputStream fileOut = context.openFileOutput(filepath, Context.MODE_PRIVATE);
            ObjectOutputStream objectOut = new ObjectOutputStream(fileOut);
            objectOut.writeObject(users);
            objectOut.close();
            return true;
        } catch (Exception ex) {
            return false;
        }
    }
}
