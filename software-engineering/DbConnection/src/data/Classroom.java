package data;

import java.util.List;

public class Classroom {
    private int id;
    private int departmentId;
    private List<Course> courses;
    private int[][] availabilityMatrix;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getDepartmentId() {
        return departmentId;
    }

    public void setDepartmentId(int departmentId) {
        this.departmentId = departmentId;
    }

    public List<Course> getCourses() {
        return courses;
    }

    public void setCourses(List<Course> courses) {
        this.courses = courses;
    }

    public int[][] getAvailabilityMatrix() {
        return availabilityMatrix;
    }

    public void setAvailabilityMatrix(int[][] availabilityMatrix) {
        this.availabilityMatrix = availabilityMatrix;
    }
}
