## Attendance Recording Software

This application is designed to record and manage the attendance of students from different classes. It provides the following functionality:

1. **Class Management**:
   - The user can insert a new class (table) where students and their attendance records can be added.
   - Classes can also be deleted or renamed.

2. **Student Management**:
   - For each class, student names can be added manually or imported from a CSV file. The CSV file should have "Name" as the first column.
   - Students can be added or removed after they’ve been inserted.

3. **Attendance Recording**:
   - The user can record attendance for all students in a class on a specific date and time using a second UI.
   - The attendance will be stored in a new column, with the column name reflecting the date and time.

4. **Attendance Management**:
   - The user can remove a specific day’s attendance (i.e., an attendance column).

5. **Class Viewing**:
   - The user can select a class via a combo box, and the selected class will be displayed in a table view.

6. **Data Editing**:
   - Data can be edited directly from the table view. A Radio Button enables or disables editing.

7. **Search Functionality**:
   - The user can search for a student's name within a class.

8. **Data Export**:
   - The user can export the attendance table for a class to a CSV file.

The application utilizes SQL databases and interacts with them using Qt for most of its functionality.
