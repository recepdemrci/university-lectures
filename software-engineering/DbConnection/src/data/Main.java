package data;

import java.sql.Date;
import java.sql.SQLException;
import java.util.ArrayList;

public class Main {

    public static void main(String[] args){
        DbConnection db = new DbConnection();
        db.initialize();


        // Create Department
		// Create Instructor
		// Add intructor into department
		// Create course
		// Make necessary process...
		
        try {
            db.inserDepartment(department);
			db.insertInstructor(instructor)
        } catch (SQLException e) {
            e.printStackTrace();
        }

        db.close_connection();
    }
}
