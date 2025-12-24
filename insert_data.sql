INSERT INTO groups (name, faculty) VALUES
('ИВТ-21', 'ФИТ'),
('ИВТ-22', 'ФИТ'),
('ЭЛ-21',  'Энергетический факультет');

INSERT INTO teachers (full_name, department) VALUES
('Сидоров Андрей Павлович', 'Кафедра ИТ'),
('Кузнецова Марина Олеговна', 'Кафедра Математики'),
('Петров Илья Сергеевич', 'Кафедра Физики');

INSERT INTO disciplines (name, credits, teacher_id) VALUES
('Базы данных', 4, 1),
('Математический анализ', 5, 2),
('Физика', 4, 3),
('ОУП (C++)', 5, 1);

INSERT INTO semesters (name, academic_year, start_date, end_date) VALUES
('1 семестр', '2024/2025', '2024-09-01', '2024-12-31'),
('2 семестр', '2024/2025', '2025-02-01', '2025-06-30');

INSERT INTO students (student_code, full_name, enrollment_year, group_id) VALUES
('ST001', 'Иванов Иван Иванович', 2024, 1),
('ST002', 'Петров Пётр Петрович', 2024, 1),
('ST003', 'Смирнова Анна Сергеевна', 2024, 2),
('ST004', 'Ким Алексей Денисович', 2024, 3);

-- Оценки (student + discipline + semester)
INSERT INTO grades (student_id, discipline_id, semester_id, grade, graded_at) VALUES
(1, 1, 1, 5, '2024-10-10 12:00:00'),
(2, 1, 1, 4, '2024-10-10 12:05:00'),
(3, 2, 1, 5, '2024-10-12 09:00:00'),
(1, 4, 1, 4, '2024-11-01 14:30:00'),
(4, 3, 1, 3, '2024-11-05 10:00:00');

-- Посещаемость
INSERT INTO attendance (student_id, discipline_id, semester_id, lesson_date, present) VALUES
(1, 1, 1, '2024-09-05', TRUE),
(2, 1, 1, '2024-09-05', FALSE),
(3, 2, 1, '2024-09-06', TRUE),
(1, 4, 1, '2024-09-10', TRUE),
(4, 3, 1, '2024-09-11', FALSE);
--