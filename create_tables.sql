DROP TABLE IF EXISTS attendance;
DROP TABLE IF EXISTS grades;
DROP TABLE IF EXISTS semesters;
DROP TABLE IF EXISTS disciplines;
DROP TABLE IF EXISTS teachers;
DROP TABLE IF EXISTS students;
DROP TABLE IF EXISTS groups;

-- ТЕМА: Учёт студентов и успеваемости

-- Группа
CREATE TABLE groups (
    group_id SERIAL PRIMARY KEY,
    name VARCHAR(20) NOT NULL UNIQUE,
    faculty VARCHAR(100) NOT NULL
);

-- Студент
CREATE TABLE students (
    student_id SERIAL PRIMARY KEY,
    student_code VARCHAR(10) NOT NULL UNIQUE,      -- например: ST001
    full_name VARCHAR(150) NOT NULL,
    enrollment_year INTEGER CHECK (enrollment_year >= 2000),
    group_id INTEGER REFERENCES groups(group_id) ON DELETE SET NULL
);

-- Преподаватель
CREATE TABLE teachers (
    teacher_id SERIAL PRIMARY KEY,
    full_name VARCHAR(150) NOT NULL,
    department VARCHAR(100) NOT NULL
);

-- Дисциплина (закреплена за преподавателем)
CREATE TABLE disciplines (
    discipline_id SERIAL PRIMARY KEY,
    name VARCHAR(120) NOT NULL UNIQUE,
    credits INTEGER CHECK (credits > 0),
    teacher_id INTEGER REFERENCES teachers(teacher_id) ON DELETE SET NULL
);

-- Семестр/поток
CREATE TABLE semesters (
    semester_id SERIAL PRIMARY KEY,
    name VARCHAR(30) NOT NULL UNIQUE,              -- например: "1 семестр"
    academic_year VARCHAR(9) NOT NULL,             -- например: "2024/2025"
    start_date DATE NOT NULL,
    end_date DATE NOT NULL,
    CHECK (start_date < end_date)
);

-- Оценка (student + discipline + semester)
CREATE TABLE grades (
    grade_id SERIAL PRIMARY KEY,
    student_id INTEGER REFERENCES students(student_id) ON DELETE CASCADE,
    discipline_id INTEGER REFERENCES disciplines(discipline_id) ON DELETE CASCADE,
    semester_id INTEGER REFERENCES semesters(semester_id) ON DELETE CASCADE,
    grade INTEGER NOT NULL CHECK (grade BETWEEN 2 AND 5),
    graded_at TIMESTAMP NOT NULL DEFAULT NOW(),
    UNIQUE (student_id, discipline_id, semester_id)
);

-- Посещаемость (по датам занятий)
CREATE TABLE attendance (
    attendance_id SERIAL PRIMARY KEY,
    student_id INTEGER REFERENCES students(student_id) ON DELETE CASCADE,
    discipline_id INTEGER REFERENCES disciplines(discipline_id) ON DELETE CASCADE,
    semester_id INTEGER REFERENCES semesters(semester_id) ON DELETE CASCADE,
    lesson_date DATE NOT NULL,
    present BOOLEAN NOT NULL DEFAULT TRUE,
    UNIQUE (student_id, discipline_id, semester_id, lesson_date)
);
--