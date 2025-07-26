# ACSQL - Secretariat System

## Overview
The ACSQL system is designed to manage university data related to students, courses, and their enrollments. It provides functionality to query, update, delete, and securely store the information using a simple encryption scheme.

---

## Data Structures

### Secretariat
The core database structure containing:
- List of all students (`studenti`)
- List of all courses (`materii`)
- Enrollment details linking students to courses (`inrolari`)

### Student
Fields:
- `id` - Unique student identifier
- `nume` - Full name of the student
- `an_studiu` - Year of study (1 to 4)
- `statut` - Type of study program ('b' for budget/state-funded, 't' for tax/self-funded)
- `medie_generala` - General average grade

### Course (Materie)
Fields:
- `id` - Unique course ID
- `nume` - Course name
- `nume_titular` - Lead instructor's name

### Enrollment (Înrolare)
Represents a many-to-many relationship between students and courses:
- `id_student` - Student ID
- `id_materie` - Course ID
- `note` - Grades array: `[lab, midterm, final]`

---

## Tasks

### 1. Database Management (10 points)

- **1.1 Load database from file:** Function to read and load the database dynamically.
- **1.2 Add student:** Add a new student to the database.
- **1.3 Free memory:** Properly free allocated memory to prevent leaks.

### 2. Queries, Updates, Deletes (70 points)

- **2.1 Queries:**
  - Simple queries (SELECT fields FROM table;)
  - Filtered queries (WHERE condition)
  - Complex filtered queries (two conditions joined by AND)

- **2.2 Updates & Deletes:**
  - Update fields (except IDs) based on conditions.
  - Delete entries based on conditions.

### 3. Database Encryption (20 points)

- Implement a simplified Cipher Block Chaining (CBC) encryption on the students array before saving.
- Use a substitution-permutation network (SPN) with an S-box (XOR with key) and a P-box permutation.
- Split data into 4 equal-sized blocks, pad last block if necessary.
- XOR first block with IV, subsequent blocks XOR’d with previous ciphertext block.

---

## Encryption Example

Message: "Ana are mere!"  
Key: `pclp1`  
IV: `ab`

Steps:
1. Convert message to bytes.
2. Split into 4 blocks and pad last block.
3. XOR first block with IV repeated.
4. Apply S-box (XOR with key).
5. Apply P-box permutation.
6. Repeat for remaining blocks, chaining with previous ciphertext.

---

## Usage

- Compile and run the ACSQL system.
- Use the provided commands for querying, updating, deleting.
- Encryption protects sensitive student data on disk.

---

## Author
Your Name

---

## License
Specify your license here.

