package data;
import java.sql.*;
import java.util.ArrayList;

/*USAGE:
    DbConnection db = new DbConnection();
    db.initialize();
    ... Some process
    db.close_connection();
*/
/*
NOTE: 
	When you insertDepartment Departmen should have Id, 
	But other insert operations musn't have Id, Their id will be given by database automaticly
*/

public class DbConnection {
    private String dbUrl;
    private String dbUsername;
    private String dbPassword;
    private Connection dbConnection;

    public DbConnection(){
        this.dbUrl =  "jdbc:sqlserver://WINDOWS\\SQLEXPRESS;databaseName=LanguageSchool";
        this.dbUsername = "language_school";
        this.dbPassword = "password";
    }

    public DbConnection(String db_url, String db_username, String db_password) {
        this.dbUrl = db_url;
        this.dbUsername = db_username;
        this.dbPassword = db_password;
    }


    public void initialize(){
        try {
            this.dbConnection = DriverManager.getConnection(dbUrl, dbUsername, dbPassword);
            System.out.println("[INFO] : Connected to database successfully");
        } catch (SQLException e) {
            System.out.println("[ERROR] : Could not connected to the database");
            e.printStackTrace();
        }
    }

    public void close_connection(){
        try {
            dbConnection.close();
            System.out.println("[INFO] : Database connection closed");
        } catch (SQLException e) {
            System.out.println("[ERROR] : Database connection could not closed");
            e.printStackTrace();
        }
    }

    // INSERT FUNCTIONS
    public void insertDepartment(Department department) throws SQLException {
        String query;
        int affectedRows;
        Statement statement = dbConnection.createStatement();

        // Fill Department table with department information
        query = String.format("INSERT INTO Department" +
                        " VALUES(%d, '%s', '%s', '%s', '%s')", department.getId(),
                                                               department.getName(),
                                                               department.getAddress(),
                                                               department.getTransport(),
                                                               department.getSocialOpportunities());
        affectedRows = statement.executeUpdate(query);

        if(affectedRows == 0){
            throw new SQLException("[ERROR] : Insert Department failed");
        }
        else {
            System.out.println("[INFO] : Department inserted successfully");
        }
        statement.close();
    }

    public void insertInstructor(Instructor instructor) throws SQLException {
        String query;
        ResultSet rs;
        int affectedRows;
        Statement statement = dbConnection.createStatement();

        // Insert Instructor into Instructor table
        query = String.format("INSERT INTO Instructor (Name, Surname, [Phone Number], [Start Date])" +
                        " VALUES('%s', '%s', '%s', '%s')", instructor.getName(),
                                                           instructor.getSurname(),
                                                           instructor.getPhoneNumber(),
                                                           instructor.getStartDateToString());
        affectedRows = statement.executeUpdate(query, Statement.RETURN_GENERATED_KEYS);

        // If insertion is successfully completed, get instructor Id from table
        if(affectedRows == 0){
            throw new SQLException("[ERROR] : Insert instructor failed");
        }
        else {
            rs = statement.getGeneratedKeys();
            if (rs.next()) {
                instructor.setId(rs.getInt(1));

                insertDepartmentInstructor(instructor);
                insertInstructorAvailabilityTime(instructor);
            }
            else {
                throw new SQLException("[ERROR] : Insert instructor failed");
            }
            System.out.println("[INFO] : Instructor inserted successfully");
        }
        statement.close();
    }

    public void insertDepartmentInstructor(Instructor instructor) throws SQLException {
        String query;
        int affectedRows;
        Statement statement = dbConnection.createStatement();

        // Insert all departments and languages of Instructor into DepartmentInstructor table
        if(instructor.getDepartments() != null) {
            for (Department department : instructor.getDepartments()) {
                for (int language : instructor.getLanguages()) {
                    query = String.format("INSERT INTO DepartmentInstructor " +
                            "VALUES (%d, %d, %d)", department.getId(), instructor.getId(), language);
                    affectedRows = statement.executeUpdate(query);

                    if (affectedRows == 0) {
                        throw new SQLException("[ERROR] : Insert DepartmentInstructor failed");
                    }
                }
            }
        }
        statement.close();
    }

    public void insertInstructorAvailabilityTime(Instructor instructor) throws SQLException {
        int i, j, k;
        int [][] availabilityMatrix;
        String query;
        int affectedRows;
        Statement statement = dbConnection.createStatement();

        availabilityMatrix = instructor.getAvailabilityMatrix();
        // Insert available time of Instructor into InstructorAvailableTime table
        for(i = 0; i < 7; i++){
            for(j = 0; j < 8; j++){
                k = availabilityMatrix[i][j];
                    query = String.format("INSERT INTO InstructorAvailableTime " +
                            "VALUES (%d, %d, %d, %d)", instructor.getId(), i, j, k);
                    affectedRows = statement.executeUpdate(query);

                    if(affectedRows == 0){
                        throw new SQLException("[ERROR] : Insert InstructorAvailabilityTime failed");
                    }
            }
        }
        statement.close();
    }

    public void insertCourse(Course course) throws SQLException {
        String query;
        int affectedRows;
        ResultSet rs;
        Statement statement = dbConnection.createStatement();

        // Fill Course table with course information
        query = String.format("INSERT INTO Course (Instructor Id, Department Id, Language, Level)" +
                        " VALUES(%d, %d, %d, %d)", course.getInstructor().getId(),
                                                   course.getClassroom().getDepartmentId(),
                                                   course.getLanguage(),
                                                   course.getLevel());
        affectedRows = statement.executeUpdate(query);

        // If insertion is successfully completed, get course Id from table
        if(affectedRows == 0){
            throw new SQLException("[ERROR] : Insert Course failed");
        }
        else {
            rs = statement.getGeneratedKeys();
            if (rs.next()) {
                course.setId(rs.getInt(1));

                insertCourseTime(course);
            }
            System.out.println("[INFO] : Course inserted successfully");
        }
        statement.close();
    }

    public void insertCourseTime(Course course) throws SQLException {
        int i, j, k;
        int [][] courseTimeMatrix;
        String query;
        int affectedRows;
        Statement statement = dbConnection.createStatement();

        courseTimeMatrix = course.getCourseTime();
        for(i = 0; i < 7; i++){
            for(j = 0; j < 8; j++){
                k = courseTimeMatrix[i][j];
                query = String.format("INSERT INTO CourseTime " +
                        "VALUES (%d, %d, %d, %d)", course.getId(), i, j, k);
                affectedRows = statement.executeUpdate(query);

                if(affectedRows == 0){
                    throw new SQLException("[ERROR] : Insert CourseTime failed");
                }

            }
        }
        statement.close();
    }

    public void insertStudent(Student student) throws SQLException {
        ArrayList<Integer> courseIds = new ArrayList<Integer>();
        String query;
        int affectedRows;
        ResultSet rs;
        Statement statement = dbConnection.createStatement();

        // Fill Student table with student information
        query = String.format("INSERT INTO Student (Name, Surname, [Phone Number], Debt, Installment)" +
                        " VALUES('%s', '%s', '%s', %f, %d)", student.getName(),
                                                                 student.getSurname(),
                                                                 student.getPhoneNumber(),
                                                                 student.getDebt(),
                                                                 student.getInstallment());
        affectedRows = statement.executeUpdate(query);

        // If insertion is successfully completed, get Student Id from table
        if(affectedRows == 0){
            throw new SQLException("[ERROR] : Insert Student failed");
        }
        else {
            rs = statement.getGeneratedKeys();
            if (rs.next()) {
                student.setId(rs.getInt(1));

                // Insert all courses into StudentCourse table which student enrolled
                if(student.getCourses() != null){
                    for (Course course : student.getCourses()){
                        courseIds.add(course.getId());
                    }
                    insertStudentCourse(student.getId(), courseIds);
                }
            }
            System.out.println("[INFO] : Student inserted successfully");
        }
        statement.close();
    }

    // Insert given course Id list into StudentCourse table
    public void insertStudentCourse(int studentId,  ArrayList<Integer> courseIds) throws SQLException {
        String query;
        int affectedRows;
        Statement statement = dbConnection.createStatement();

        // Insert into StudentCourse table with courses which enrolled by student
        for (int courseId : courseIds){
            query = String.format("INSERT INTO StudentCourse VALUES(%d, %d)", studentId, courseId);
            affectedRows = statement.executeUpdate(query);

            if(affectedRows == 0){
                throw new SQLException("[ERROR] : Insert StudentCourse failed");
            }
        }
        statement.close();
    }


    // DELETE FUNCTIONS ------------------------------------------------------------------------------------------------

    // Delete Department
    public void deleteDepartment(int Id) {
        String query;
        Statement statement = null;

        try {
            statement = dbConnection.createStatement();
            query = String.format("DELETE FROM Department WHERE Id = %d", Id);
            statement.executeUpdate(query);

            System.out.println("[INFO] : Department deleted successfully");

        } catch (SQLException e) {
            System.out.println("[ERROR] : Delete Department failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Delete Instructor
    public void deleteInstructor(int Id) {
        String query;
        Statement statement = null;

        try {
            statement = dbConnection.createStatement();
            query = String.format("DELETE FROM Instructor WHERE Id = %d", Id);
            statement.executeUpdate(query);

            System.out.println("[INFO] : Instructor deleted successfully");

        } catch (SQLException e) {
            System.out.println("[ERROR] : Delete Instructor failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Delete DepartmentInstructor
    // NOTE: It can not use out of this class
    // (If you want to change any department from Instructor, Use updateInstructor)
    public void deleteDepartmentInstructor(int instructorId, ArrayList<Integer> departmentIds){
        String query;
        Statement statement = null;

        try {
            statement = dbConnection.createStatement();

            for (int departmentId : departmentIds) {
                query = String.format("DELETE FROM DepartmentInstructor WHERE [Department Id] = %d", departmentId);
                statement.executeUpdate(query);
            }

        } catch (SQLException e) {
            System.out.println("[ERROR] : Delete DepartmentInstructor failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Delete Course
    public void deleteCourse(int Id) {
        String query;
        Statement statement = null;

        try {
            statement = dbConnection.createStatement();
            query = String.format("DELETE FROM Course WHERE Id = %d", Id);
            statement.executeUpdate(query);
            System.out.println("[INFO] : Course deleted successfully");
        } catch (SQLException e){
            System.out.println("[ERROR] : Delete Course failed");
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Delete Student
    public void deleteStudent(int Id) {
        String query;
        Statement statement = null;

        try {
            statement = dbConnection.createStatement();
            query = String.format("DELETE FROM Student WHERE Id = %d", Id);
            statement.executeUpdate(query);

            System.out.println("[INFO] : Student deleted successfully");

        } catch (SQLException e) {
            System.out.println("[ERROR] : Delete Student failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Delete StudentCourse
    // NOT: It can not use ou of this class
    // (If you want to change any course from student, Use updateStudent)
    public void deleteStudentCourse(int studentId, ArrayList<Integer> courseIds){
        String query;
        Statement statement = null;

        try {
            statement = dbConnection.createStatement();

            for (int courseId : courseIds) {
                query = String.format("DELETE FROM StudentCourse WHERE [Course Id] = %d", courseId);
                statement.executeUpdate(query);
            }

        } catch (SQLException e) {
            System.out.println("[ERROR] : Delete StudentCourse failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }


    // UPDATE FUNCTIONS ------------------------------------------------------------------------------------------------

    // Update general information in Department table
    public void updateDepartment(Department department) {
        String query;
        PreparedStatement statement = null;

        try {
            query = "UPDATE Department SET Name=?, Address=?, Transport=?, [Social Opportunities]=? WHERE Id=?";
            statement = dbConnection.prepareStatement(query);
            statement.setString(1, department.getName());
            statement.setString(2, department.getAddress());
            statement.setString(3, department.getTransport());
            statement.setString(4, department.getSocialOpportunities());
            statement.setInt(5, department.getId());
            statement.executeUpdate();

            System.out.println("[INFO] : Existing department updated successfully");
        } catch (SQLException e) {
            System.out.println("[ERROR] : Update Department failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Update Instructor table and related tables(DepartmentInstructor, InstructorAvailability)
    // (NOTE: If Instructor learns new language that is not included in this function)
    public void updateInstructor(Instructor instructor) {
        String query;
        PreparedStatement statement = null;

        try {
            // Update general information about instructor
            query = "UPDATE Instructor SET Name=?, Surname=?, [Phone Number]=?, [Start Date]=? WHERE Id=?";
            statement = dbConnection.prepareStatement(query);
            statement.setString(1, instructor.getName());
            statement.setString(2, instructor.getSurname());
            statement.setString(3, instructor.getPhoneNumber());
            statement.setString(4, instructor.getStartDateToString());
            statement.setInt(5, instructor.getId());
            statement.executeUpdate();

            // Update related table with Instructor
            updateDepartmentInstructor(instructor);
            insertInstructorAvailabilityTime(instructor);

            System.out.println("[INFO] : Existing instructor updated successfully");
        } catch (SQLException e) {
            System.out.println("[ERROR] : Update instructor failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Update DepartmentInstructor
    // Insert Departments which is new in the instructor
    // Delete Departments which is not in the instructor
    public void updateDepartmentInstructor(Instructor instructor)    {
        ArrayList<Integer> dbDepartmentIds = new ArrayList<Integer>();       // Department Ids from database
        ArrayList<Integer> departmentIds = new ArrayList<Integer>();         // Department Ids from given departments
        ArrayList<Integer> deleteList;                                       // Department Ids which will delete from db
        ArrayList<Integer> insertList;                                       // Department Ids which will insert into db

        String query;
        Statement statement = null;
        ResultSet rs = null;

        try {
            statement = dbConnection.createStatement();

            // Create a list of department Ids which is in the database
            query = String.format("SELECT [Department Id] FROM DepartmentInstructor WHERE [Instructor Id] = %d", instructor.getId());
            rs = statement.executeQuery(query);
            while (rs.next()){
                dbDepartmentIds.add(rs.getInt("[Department Id]"));
            }

            // Create a list of department Ids which is in the departments of instructor
            for (Department department : instructor.getDepartments()){
                departmentIds.add(department.getId());
            }

            // Calculate differences between two list and find which rows will be insert or delete
            deleteList = (ArrayList<Integer>) dbDepartmentIds.clone();
            insertList = (ArrayList<Integer>) departmentIds.clone();
            deleteList.removeAll(departmentIds);
            insertList.removeAll(dbDepartmentIds);

            // Insert and delete departments from database
            deleteDepartmentInstructor(instructor.getId(), deleteList);

            for (int departmentId : insertList) {
                for (int language : instructor.getLanguages()){
                    query = String.format("INSERT INTO DepartmentInstructor VALUES (%d, %d, %d)", departmentId
                                                                                                , instructor.getId()
                                                                                                , language);
                    statement.executeUpdate(query);
                }
            }

            System.out.println("[INFO] : DepartmentInstructor updated successfully");
        } catch (SQLException e) {
            System.out.println("[ERROR] : Update DepartmentInstructor failed");
            e.printStackTrace();
        } finally {
            try { if (rs != null) rs.close(); } catch (SQLException e) { e.printStackTrace(); }
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Update Course table and related tables(CourseTime)
    public void updateCourse(Course course) {
        String query;
        PreparedStatement statement = null;

        try {
            // Update general information about Course
            query = "UPDATE Course SET [Instructor Id]=?, [Department Id]=?, Language=?, Level=? WHERE Id=?";
            statement = dbConnection.prepareStatement(query);
            statement.setInt(1, course.getInstructor().getId());
            statement.setInt(2, course.getClassroom().getDepartmentId());
            statement.setInt(3, course.getLanguage());
            statement.setInt(4, course.getLevel());
            statement.setInt(5, course.getId());
            statement.executeUpdate();

            // Update related table with Course
            insertCourseTime(course);

            System.out.println("[INFO] : Existing Course updated successfully");
        } catch (SQLException e) {
            System.out.println("[ERROR] : Update Course failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Update Student table and related tables(StudentCourse table)
    public void updateStudent(Student student){
        String query;
        PreparedStatement statement = null;

        try {
            // Update general information about Student
            query = "UPDATE Student SET Name=?, Surname=?, [Phone Number]]=?, Debt=?, Installment=? WHERE Id=?";
            statement = dbConnection.prepareStatement(query);
            statement.setString(1, student.getName());
            statement.setString(2, student.getSurname());
            statement.setString(3, student.getPhoneNumber());
            statement.setFloat(4, student.getDebt());
            statement.setInt(5, student.getInstallment());
            statement.setInt(6, student.getId());
            statement.executeUpdate();

            // Update related table with Student
            updateStudentCourse(student);

            System.out.println("[INFO] : Existing Student updated successfully");
        } catch (SQLException e) {
            System.out.println("[ERROR] : Update Student failed");
            e.printStackTrace();
        } finally {
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }

    // Update StudentCourse table
    // Insert courses which is new in the student and
    // Delete courses which is not in the student anymore
    public void updateStudentCourse(Student student){
        ArrayList<Integer> dbCourseIds = new ArrayList<Integer>();       // Department Ids from database
        ArrayList<Integer> courseIds = new ArrayList<Integer>();         // Department Ids from given departments
        ArrayList<Integer> deleteList;                                       // Department Ids which will delete from db
        ArrayList<Integer> insertList;                                       // Department Ids which will insert into db

        String query;
        Statement statement = null;
        ResultSet rs = null;

        try {
            statement = dbConnection.createStatement();

            // Create a list of Course Ids which is in the database
            query = String.format("SELECT [Course Id] FROM StudentCourse WHERE [Studentt Id] = %d", student.getId());
            rs = statement.executeQuery(query);
            while (rs.next()){
                dbCourseIds.add(rs.getInt("[Course Id]"));
            }

            // Create a list of course Ids which is in the students courses
            for (Course course : student.getCourses()){
                courseIds.add(course.getId());
            }

            // Calculate differences between two list and find which rows will be insert or delete
            deleteList = (ArrayList<Integer>) dbCourseIds.clone();
            insertList = (ArrayList<Integer>) courseIds.clone();
            deleteList.removeAll(courseIds);
            insertList.removeAll(dbCourseIds);

            // Insert and delete courses from database
            deleteStudentCourse(student.getId(), deleteList);
            insertStudentCourse(student.getId(), insertList);

            System.out.println("[INFO] : StudentCourse updated successfully");
        } catch (SQLException e) {
            System.out.println("[ERROR] : Update StudentCourse failed");
            e.printStackTrace();
        } finally {
            try { if (rs != null) rs.close(); } catch (SQLException e) { e.printStackTrace(); }
            try { if (statement != null) statement.close(); } catch (SQLException e) { e.printStackTrace(); }
        }
    }
}