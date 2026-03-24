This is a C program that uses files as a database for a scalable educational institution management system (a prototype version).

· Relationships: each subject is taught by one teacher, and each student is enrolled in only one subject.
· Deletion is logical, using an activity flag (1: active, 0: deleted).

Entities and Permissions:

1. Admin (id=1, pass='a'):
   · Sole permission: reset the program by deleting a specific type of file entirely.
2. Principal (id=0, pass='d'):
   · Permissions: add/delete student, view student list, view job applications, process applications (accept/reject), add/delete teacher, update application list.
   · Adding a teacher is done by accepting an application from the job applications file, then generating an ID and password for the teacher.
   · Updating applications: marks processed applications as "done" to avoid interference.
3. Job Applications:
   · An application includes key information and is saved with a composite ID (id+key).
   · To check an application: enter the composite ID; if accepted, the teacher receives their login credentials.
4. Teacher:
   · Logs in using their assigned ID and password.
   · Functions: view personal info card, enter grades for students enrolled in their subject, view only their own student list.
5. Student:
   · Logs in using their assigned ID.
   · Functions: view personal information and grades for the subject they are enrolled in.

Files Used:

· Grades file: stores (student ID, grade, subject name, teacher ID).
· Login file: stores teacher credentials (ID and password).
· Admin and principal credentials are hardcoded.
· Project structure: a folder named school containing the program and all data files.
