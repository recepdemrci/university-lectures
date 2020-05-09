package data;

import java.util.List;

public class Department {
    private int id;
    private String name;
    private String address;
    private String transport;
    private String socialOpportunities;
    private List<Classroom> classrooms;
    private List<Instructor> instructors;
    private List<Integer> languages;
    private List<Student> students;

    public Department(int id, String name, String address, String transport, String socialOpportunities) {
        this.id = id;
        this.name = name;
        this.address = address;
        this.transport = transport;
        this.socialOpportunities = socialOpportunities;
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

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getTransport() {
        return transport;
    }

    public void setTransport(String transport) {
        this.transport = transport;
    }

    public String getSocialOpportunities() {
        return socialOpportunities;
    }

    public void setSocialOpportunities(String socialOpportunities) {
        this.socialOpportunities = socialOpportunities;
    }

    public List<Classroom> getClassrooms() {
        return classrooms;
    }

    public void setClassrooms(List<Classroom> classrooms) {
        this.classrooms = classrooms;
    }

    public List<Instructor> getInstructors() {
        return instructors;
    }

    public void setInstructors(List<Instructor> instructors) {
        this.instructors = instructors;
    }

    public List<Integer> getLanguages() {
        return languages;
    }

    public void setLanguages(List<Integer> languages) {
        this.languages = languages;
    }

    public List<Student> getStudents() {
        return students;
    }

    public void setStudents(List<Student> students) {
        this.students = students;
    }
}
