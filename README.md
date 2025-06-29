# ACSQL
**Secretariat System – Overview**  
  The secretariat has decided to implement a new database to manage information about students, courses, and the relationships between them. The new database will be handled   via a query and update system called ACSQL.

  To achieve this, your help has been requested to design and implement the core functionalities of the system.

**Data Structures**  
  The proposed system will be modeled using the following structures:

1.Secretariat
  This structure represents the main database and centralizes:

  The list of all students enrolled at the university.

  The list of all courses taught at the university.

  The enrollment details linking students to courses.

    typedef struct secretariat {
      student *studenti;     // Array of students
      int nr_studenti;       // Total number of students

      materie *materii;      // Array of courses
      int nr_materii;        // Total number of courses

      inrolare *inrolari;    // Array of enrollments (student-course relations)
      int nr_inrolari;       // Total number of enrollments
    } secretariat;

2.Student
  The student structure describes each student enrolled in the university and contains the following fields:

  id: A unique identifier for each student.

  nume: The full name of the student.

  an_studiu: The student’s year of study (1, 2, 3, or 4).
  
  statut: The type of study program:
  
  'b' for budget (state-funded)
  
  't' for tax (self-funded)
  
  medie_generala: The student’s overall average grade, calculated based on all course grades.

       typedef struct student {
          int id;                // Unique student ID
          char nume[40];         // Full name
          int an_studiu;         // Year of study
          char statut;           // 'b' (budget) or 't' (tax)
          float medie_generala;  // General average grade
        } student;
   
3.Course (Materie)
    The materie structure contains details about each course taught at the university:

  id: A unique course ID

  nume: Name of the course
  
  nume_titular: Name of the lead instructor

        typedef struct materie {
           int id;                // Unique course ID
          char *nume;            // Course name
          char *nume_titular;    // Instructor name
        } materie;

4. Enrollment (Înrolare)
    The inrolare structure represents the many-to-many relationship between students and courses. Each enrollment contains:

    id_student: The ID of the student enrolled in the course

    id_materie: The ID of the enrolled course

    note: A 3-element array of grades:

    Grade for lab and assignments

    Midterm exam grade

    Final exam grade

        typedef struct inrolare {
          int id_student;    // Student ID
          int id_materie;    // Course ID
          float note[3];     // Grades: [lab, midterm, final]
        } inrolare;

**Task 1: Database Management (10 points)**  
*1.1 Load the database from a file (5 points)*  
Write a function that reads data from a file organized as described above and loads it into a dynamically allocated secretariat structure:

    secretariat *read_secretariat(const char *filename);
*1.2 Add a student (3 points)*  
    Write a function that adds a student to the given database:

    void add_student(secretariat *s, int id, char *name, int year, char status, float gpa);

*1.3 Free memory (2 points)*  
To prevent memory leaks, implement a function that frees all dynamically allocated memory for the secretariat structure and its contents:

    void free_secretariat(secretariat **s);

**Task 2: Queries, Updates, Deletes (70 points)**  
*2.1 Queries (28 points)*  
  2.1.1 Simple Queries (14 points)
  A simple query has the form:

    SELECT <fields> FROM <table>;
  <fields> is a comma-separated list of the fields to display (or * for all fields).

  <table> is one of studenti, materii, or inrolari.

  The query function should print, line by line, the selected fields from each row of the specified table.

Example:


    SELECT name, year FROM studenti;
Output for the example database above:

    Andrei Popescu 2
    Ioana Ionescu 1
Using * to select all fields:

    SELECT * FROM materii;
Output:

    0 PCLP Radu Bran
    1 USO Maria Sandu
Selecting the grades array from inrolari should print all three grades, separated by spaces:

    SELECT student_id, grades FROM inrolari;
Output:

    1 3.1 3.8 2.1
    2 2.65 1.20 3.0

*2.1.2 Filtered Queries (7 points)*  
A filtered query has the form:

    SELECT <fields> FROM <table> WHERE <field> <operator> <value>;
<operator> may be one of =, <, >, <=, >=, !=.

Example:

    SELECT name, gpa FROM studenti WHERE year > 2;
Output:

    Andrei Popescu 8.72

*2.1.3 Complex Filtered Queries (7 points)*  
A complex filtered query allows two conditions joined by AND:

    SELECT <fields> FROM <table> WHERE <cond1> AND <cond2>;
Example:

    SELECT name, gpa FROM studenti WHERE year > 1 AND status = 'b';
Output:

    Andrei Popescu 8.72

**2.2 Updates & Deletes (42 points)**
*2.2.1 UPDATE (21 points)*
An update command has the form:
  
    UPDATE <table> SET <field> = <value> WHERE <condition>;
You may not update ID fields.

Example:

    UPDATE studenti SET gpa = 9.0 WHERE id = 0;
    SELECT name, gpa FROM studenti WHERE id = 0;
Output:

    Andrei Popescu 9.0
Updating the grades field in inrolari:

    UPDATE inrolari SET grades = 4.0 4.0 1.0 WHERE student_id = 2;

*2.2.2 DELETE (14 points)*
A delete command has the form:

    DELETE FROM <table> WHERE <condition>;
Example:

    DELETE FROM studenti WHERE id = 0;
    SELECT name, gpa FROM studenti;
Output:

    Ioana Ionescu 7.98
After implementing all individual commands, there will be a mixed test worth 7 points.

**Task 3: Database Encryption (20 points)**
To protect the data in the secretariat’s database, the students array must be encrypted before it is written to disk. You will implement a simplified Cipher Block Chaining (CBC) scheme.

In CBC, the byte-vector representing all students is split into 4 equal-sized blocks, padding the last block with 0x00 bytes if necessary. Each block is then encrypted in sequence using a simple substitution-permutation network (S-box and P-box).

Cipher Block Chaining (CBC)
Common block ciphers operate on fixed-size blocks. If you encrypt each block independently (Electronic Codebook, ECB), identical plaintext blocks yield identical ciphertext blocks, revealing patterns. CBC avoids this by chaining: the first block is XOR’d with an Initialization Vector (IV) before encryption, and each subsequent block is XOR’d with the previous ciphertext block.

If the IV is shorter than the block size, repeat it to fill the block.

If the total data length is not a multiple of 4 blocks, pad the final block with 0x00 bytes.

Substitution–Permutation Network
A simple SPN applies two steps to each block:

S-box (Substitution): XOR each byte of the block with a key byte. If the key is shorter than the block, repeat it as needed.

P-box (Permutation): Rearrange the bytes according to the bijection

    f(i) = (i * (n – 1) + 2) mod n
where i is the original byte index and n is the block size. Byte at position j in the output comes from the byte at the unique i such that f(i) = j.


    function encrypt_students(student_bytes, IV, key, output_path):
        // Split into 4 blocks of equal size, pad last block with 0x00 if needed
        blocks = split_and_pad(student_bytes, 4)
    
        // Encrypt block 0 with IV
        temp = XOR(blocks[0], extend_to_block_size(IV))
        temp = S_BOX(temp, key)
        blocks_enc[0] = P_BOX(temp)
    
        // Encrypt blocks 1 through 3
        for i in 1..3:
            temp = XOR(blocks[i], blocks_enc[i-1])
            temp = S_BOX(temp, key)
            blocks_enc[i] = P_BOX(temp)
    
        // Write all encrypted blocks to output_path

Encryption Process Diagram
   ![image](https://github.com/user-attachments/assets/e9342423-3846-48ad-9f00-f0664a76b266)

Example
Let’s assume we want to encrypt the message "Ana are mere!" using the key pclp1 and the IV ab.

Step 1: Convert the message to bytes

    "Ana are mere!" → 
    0x41 0x6E 0x61 0x20 0x61 0x72 0x65 0x20 0x6D 0x65 0x72 0x65 0x21
Step 2: Split into 4 blocks and pad the last block

    Block 1: 0x41 0x6E 0x61 0x20  
    Block 2: 0x61 0x72 0x65 0x20  
    Block 3: 0x6D 0x65 0x72 0x65  
    Block 4: 0x21 0x00 0x00 0x00  (padded with 0x00)
Step 3: Convert IV and key to bytes

    IV "ab" → 0x61 0x62

Key "pclp1" → 0x70 0x63 0x6C 0x70 0x31

Block 1 Encryption
XOR Block 1 with IV (repeated to fit 4 bytes):

    0x41 0x6E 0x61 0x20
    XOR
    0x61 0x62 0x61 0x62
    = 
    0x20 0x0C 0x00 0x42
Apply S-box (XOR with key):

    0x20 0x0C 0x00 0x42
    XOR
    0x70 0x63 0x6C 0x70
    =
    0x50 0x6F 0x6C 0x32
Apply P-box (f(i) = (i * 3 + 2) mod 4):

    Byte at position 0 → position 2
    
    Byte at position 1 → position 1
    
    Byte at position 2 → position 0
    
    Byte at position 3 → position 3
    
    Resulting block: 0x6C 0x6F 0x50 0x32

Block 2 Encryption
XOR with previous encrypted block (Block 1):

    0x6C 0x6F 0x50 0x32
    XOR
    0x61 0x72 0x65 0x20
    =
    0x0D 0x1D 0x35 0x12
Apply S-box and P-box as above...
   
