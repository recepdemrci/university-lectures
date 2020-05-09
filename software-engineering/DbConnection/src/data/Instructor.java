package data;

import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.ArrayList;

public class Instructor {
    private int id;
    private String name;
    private String surname;
    private String phoneNumber;
    private Date startDate;
    private ArrayList<Integer> languages;
    private ArrayList<Department> departments;
    private int[][] availabilityMatrix;


    public Instructor(String name, String surname, String phoneNumber, Date startDate, int[][] availabilityMatrix) {
        this.name = name;
        this.surname = surname;
        this.phoneNumber = phoneNumber;
        this.startDate = startDate;
        this.languages = new ArrayList<Integer>();
        this.departments = new ArrayList<Department>();
        this.availabilityMatrix = availabilityMatrix;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSurname() {
        return surname;
    }

    public void setSurname(String surname) {
        this.surname = surname;
    }

    public String getPhoneNumber() {
        return phoneNumber;
    }

    public void setPhoneNumber(String phoneNumber) {
        this.phoneNumber = phoneNumber;
    }

    public Date getStartDate() {
        return startDate;
    }

    public String getStartDateToString() {
        return new SimpleDateFormat("yyyy-MM-dd").format(this.getStartDate());
    }

    public void setStartDate(Date startDate) {
        this.startDate = startDate;
    }

    public ArrayList<Integer> getLanguages() {
        return languages;
    }

    public void setLanguages(ArrayList<Integer> languages) {
        this.languages = languages;
    }

    public ArrayList<Department> getDepartments() {
        return departments;
    }

    public void setDepartments(ArrayList<Department> departments) {
        this.departments = departments;
    }

    public int[][] getAvailabilityMatrix() {
        return availabilityMatrix;
    }

    public void setAvailabilityMatrix(int[][] availabilityMatrix) {
        this.availabilityMatrix = availabilityMatrix;
    }
}
