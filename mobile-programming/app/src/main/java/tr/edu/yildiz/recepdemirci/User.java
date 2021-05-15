package tr.edu.yildiz.recepdemirci;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.FileInputStream;
import java.io.InputStream;
import java.io.Serializable;
import java.time.LocalDate;

public class User implements Serializable {
    private static final long serialVersionUID = 1L;

    private String image_path;
    private String name;
    private String surname;
    private String email;
    private String phone;
    private LocalDate birthDate;
    private double password;

    
    public User(String email, String password) {
        this.email = email;
        this.password = hashFunction(password);
    }


    public User(String name, String surname, String email, String phone, String password, LocalDate birthDate, String image_path) {
        this.image_path = image_path;
        this.name = name;
        this.surname = surname;
        this.birthDate = birthDate;
        this.email = email;
        this.phone = phone;
        this.password = hashFunction(password);
    }

    public String getEmail() {
        return email;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
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

    private double hashFunction(String password) {
        int i;
        float sum = 0;

        String password_ = password.substring(0, Math.min(password.length(), 50));
        for (char letter : password_.toCharArray()) {
            i = letter % 13;
            sum += i;
        }
        return (sum % 7);
    }

    @Override
    public boolean equals(Object o) {
        User user = (User) o;
        return user.email.equals(this.email) && user.password == this.password;
    }
}
