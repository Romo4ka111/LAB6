#include "operations.h"

void showMenu() {
    cout << endl << string(46, '=') << endl;
    cout << "=== УЧЁТ СТУДЕНТОВ И УСПЕВАЕМОСТИ ===" << endl;
    cout << "1.  Все оценки (ведомость)" << endl;
    cout << "2.  Все студенты" << endl;
    cout << "3.  Все дисциплины" << endl;

    cout << "\n=== ПОИСК ===" << endl;
    cout << "4.  Поиск оценок по студенту (ФИО)" << endl;
    cout << "5.  Дисциплины конкретного преподавателя" << endl;

    cout << "\n=== ОПЕРАЦИИ ===" << endl;
    cout << "6.  Добавить оценку" << endl;
    cout << "7.  Изменить оценку" << endl;
    cout << "8.  Удалить оценку" << endl;

    cout << "\n=== АНАЛИТИКА ===" << endl;
    cout << "9.  Успеваемость групп (avg > 4.0 и >=3 оценки)" << endl;
    cout << "10. Максимальная оценка (кто получил)" << endl;
    cout << "11. Нагрузка преподавателей (кол-во дисциплин)" << endl;
    cout << "12. Общая статистика" << endl;

    cout << "\n=== БЕЗОПАСНОСТЬ (SQL Injection) ===" << endl;
    cout << "13. Уязвимый поиск студента" << endl;
    cout << "14. UNION инъекция (оценки)" << endl;
    cout << "15. Уязвимое удаление посещаемости" << endl;

    cout << "\n=== ПОСЕЩАЕМОСТЬ ===" << endl;
    cout << "16. Просмотр посещаемости" << endl;

    cout << "\n0.  Выход" << endl;
}

// 1) Все оценки
void viewAllFlights(pqxx::work& txn) {
    cout << "\n--- Ведомость оценок ---" << endl;

    pqxx::result res = txn.exec(
        "SELECT g.grade_id, s.student_code, s.full_name, gr.name AS group_name, "
        "d.name AS discipline, t.full_name AS teacher, sem.name AS semester, "
        "g.grade, g.graded_at "
        "FROM grades g "
        "JOIN students s ON g.student_id = s.student_id "
        "LEFT JOIN groups gr ON s.group_id = gr.group_id "
        "JOIN disciplines d ON g.discipline_id = d.discipline_id "
        "LEFT JOIN teachers t ON d.teacher_id = t.teacher_id "
        "JOIN semesters sem ON g.semester_id = sem.semester_id "
        "ORDER BY g.graded_at DESC"
    );

    for (const auto& row : res) {
        cout << "[ID " << row["grade_id"].as<int>() << "] "
             << row["student_code"].c_str() << " | "
             << row["full_name"].c_str() << " | "
             << (row["group_name"].is_null() ? "-" : row["group_name"].c_str()) << " | "
             << row["discipline"].c_str() << " | "
             << (row["teacher"].is_null() ? "-" : row["teacher"].c_str()) << " | "
             << row["semester"].c_str() << " | Оценка: "
             << row["grade"].as<int>() << endl;
    }
}

// 2) Все студенты
void viewAllAirports(pqxx::work& txn) {
    cout << "\n--- Список студентов ---" << endl;

    pqxx::result res = txn.exec(
        "SELECT s.student_code, s.full_name, s.enrollment_year, gr.name AS group_name "
        "FROM students s "
        "LEFT JOIN groups gr ON s.group_id = gr.group_id "
        "ORDER BY gr.name NULLS LAST, s.full_name"
    );

    for (const auto& row : res) {
        cout << row["student_code"].c_str() << " | "
             << row["full_name"].c_str() << " | "
             << row["enrollment_year"].as<int>() << " | Группа: "
             << (row["group_name"].is_null() ? "-" : row["group_name"].c_str())
             << endl;
    }
}

// 3) Все дисциплины
void viewAllPlanes(pqxx::work& txn) {
    cout << "\n--- Дисциплины ---" << endl;

    pqxx::result res = txn.exec(
        "SELECT d.discipline_id, d.name, d.credits, t.full_name AS teacher "
        "FROM disciplines d "
        "LEFT JOIN teachers t ON d.teacher_id = t.teacher_id "
        "ORDER BY d.name"
    );

    for (const auto& row : res) {
        cout << "[ID " << row["discipline_id"].as<int>() << "] "
             << row["name"].c_str() << " | credits: " << row["credits"].as<int>()
             << " | Преподаватель: "
             << (row["teacher"].is_null() ? "-" : row["teacher"].c_str())
             << endl;
    }
}

// 4) Поиск оценок по студенту
void searchFlightsByCity(pqxx::work& txn) {
    string fio;
    cout << "Введите ФИО (или часть): ";
    getline(cin, fio);

    pqxx::result res = txn.exec_params(
        "SELECT s.full_name, d.name AS discipline, sem.name AS semester, g.grade "
        "FROM grades g "
        "JOIN students s ON g.student_id = s.student_id "
        "JOIN disciplines d ON g.discipline_id = d.discipline_id "
        "JOIN semesters sem ON g.semester_id = sem.semester_id "
        "WHERE s.full_name ILIKE '%' || $1 || '%' "
        "ORDER BY s.full_name, d.name",
        fio
    );

    if (res.empty()) {
        cout << "Ничего не найдено." << endl;
        return;
    }

    for (const auto& row : res) {
        cout << row["full_name"].c_str()
             << " | " << row["discipline"].c_str()
             << " | " << row["semester"].c_str()
             << " | Оценка: " << row["grade"].as<int>() << endl;
    }
}

// 5) Дисциплины по преподавателю
void searchPlanesByAirline(pqxx::work& txn) {
    string teacher;
    cout << "Введите ФИО преподавателя (или часть): ";
    getline(cin, teacher);

    pqxx::result res = txn.exec_params(
        "SELECT d.name, d.credits "
        "FROM disciplines d "
        "JOIN teachers t ON d.teacher_id = t.teacher_id "
        "WHERE t.full_name ILIKE '%' || $1 || '%' "
        "ORDER BY d.name",
        teacher
    );

    if (res.empty()) {
        cout << "Не найдено дисциплин." << endl;
        return;
    }

    for (const auto& row : res) {
        cout << row["name"].c_str()
             << " | credits: " << row["credits"].as<int>() << endl;
    }
}

// 6) Добавить оценку
void registerNewPassenger(pqxx::work& txn) {
    string studentCode;
    int disciplineId, semesterId, grade;

    cout << "Код студента (например ST001): ";
    getline(cin, studentCode);

    pqxx::result st = txn.exec_params(
        "SELECT student_id FROM students WHERE student_code = $1",
        studentCode
    );
    if (st.empty()) {
        cout << "Ошибка: студент не найден." << endl;
        return;
    }
    int studentId = st[0]["student_id"].as<int>();

    cout << "ID дисциплины (см. пункт 3): ";
    cin >> disciplineId;
    cout << "ID семестра (1/2 ...): ";
    cin >> semesterId;
    cout << "Оценка (2..5): ";
    cin >> grade;
    cin.ignore();

    txn.exec_params(
        "INSERT INTO grades (student_id, discipline_id, semester_id, grade) "
        "VALUES ($1, $2, $3, $4)",
        studentId, disciplineId, semesterId, grade
    );

    cout << "Оценка добавлена." << endl;
}

// 7) Изменить оценку
void updateFlightStatus(pqxx::work& txn) {
    int gradeId, newGrade;
    cout << "ID оценки (см. пункт 1): ";
    cin >> gradeId;
    cout << "Новая оценка (2..5): ";
    cin >> newGrade;
    cin.ignore();

    pqxx::result res = txn.exec_params(
        "UPDATE grades SET grade = $1, graded_at = NOW() WHERE grade_id = $2",
        newGrade, gradeId
    );

    if (res.affected_rows() > 0) cout << "Оценка обновлена." << endl;
    else cout << "Запись не найдена." << endl;
}

// 8) Удалить оценку
void cancelFlight(pqxx::work& txn) {
    int gradeId;
    cout << "Введите ID оценки для удаления: ";
    cin >> gradeId;
    cin.ignore();

    pqxx::result res = txn.exec_params(
        "DELETE FROM grades WHERE grade_id = $1",
        gradeId
    );

    cout << "Удалено записей: " << res.affected_rows() << endl;
}

// 9) Успеваемость групп (GROUP BY + HAVING)
void showAirportsStats(pqxx::work& txn) {
    cout << "\n--- Успеваемость групп (avg > 4.0 и >= 3 оценки) ---" << endl;

    pqxx::result res = txn.exec(
        "SELECT gr.name AS group_name, "
        "COUNT(g.grade_id) AS grades_count, "
        "AVG(g.grade)::numeric(10,2) AS avg_grade "
        "FROM groups gr "
        "JOIN students s ON s.group_id = gr.group_id "
        "JOIN grades g ON g.student_id = s.student_id "
        "GROUP BY gr.name "
        "HAVING COUNT(g.grade_id) >= 3 AND AVG(g.grade) > 4.0 "
        "ORDER BY avg_grade DESC"
    );

    if (res.empty()) {
        cout << "Нет групп, подходящих под условие." << endl;
        return;
    }

    for (const auto& row : res) {
        cout << row["group_name"].c_str()
             << " | оценок: " << row["grades_count"].as<int>()
             << " | средний балл: " << row["avg_grade"].c_str()
             << endl;
    }
}

// 10) Максимальная оценка (подзапрос)
void showMostExpensiveFlight(pqxx::work& txn) {
    cout << "\n--- Максимальная оценка ---" << endl;

    pqxx::result res = txn.exec(
        "SELECT s.full_name, d.name AS discipline, sem.name AS semester, g.grade "
        "FROM grades g "
        "JOIN students s ON g.student_id = s.student_id "
        "JOIN disciplines d ON g.discipline_id = d.discipline_id "
        "JOIN semesters sem ON g.semester_id = sem.semester_id "
        "WHERE g.grade = (SELECT MAX(grade) FROM grades)"
    );

    for (const auto& row : res) {
        cout << row["full_name"].c_str()
             << " | " << row["discipline"].c_str()
             << " | " << row["semester"].c_str()
             << " | Оценка: " << row["grade"].as<int>() << endl;
    }
}

// 11) Нагрузка преподавателей (LEFT JOIN)
void showAirlinesFleet(pqxx::work& txn) {
    cout << "\n--- Нагрузка преподавателей (кол-во дисциплин) ---" << endl;

    pqxx::result res = txn.exec(
        "SELECT t.full_name, COUNT(d.discipline_id) AS disciplines_count "
        "FROM teachers t "
        "LEFT JOIN disciplines d ON d.teacher_id = t.teacher_id "
        "GROUP BY t.full_name "
        "ORDER BY disciplines_count DESC, t.full_name"
    );

    for (const auto& row : res) {
        cout << row["full_name"].c_str()
             << " | дисциплин: " << row["disciplines_count"].as<int>()
             << endl;
    }
}

// 12) Общая статистика
void showGeneralStats(pqxx::work& txn) {
    cout << "\n--- Общая статистика ---" << endl;

    pqxx::result res = txn.exec(
        "SELECT "
        "(SELECT COUNT(*) FROM students) AS total_students, "
        "(SELECT COUNT(*) FROM grades) AS total_grades, "
        "(SELECT AVG(grade)::numeric(10,2) FROM grades) AS avg_grade, "
        "(SELECT COUNT(*) FROM attendance) AS total_attendance, "
        "(SELECT CASE WHEN (SELECT COUNT(*) FROM attendance)=0 THEN 0 "
        " ELSE (SELECT (SUM(CASE WHEN present THEN 1 ELSE 0 END)::numeric / COUNT(*))::numeric(10,2) FROM attendance) END) "
        " AS attendance_rate"
    );

    cout << "Студентов: " << res[0]["total_students"].as<int>() << endl;
    cout << "Оценок: " << res[0]["total_grades"].as<int>() << endl;
    cout << "Средний балл: " << res[0]["avg_grade"].c_str() << endl;
    cout << "Записей посещаемости: " << res[0]["total_attendance"].as<int>() << endl;
    cout << "Доля присутствий: " << res[0]["attendance_rate"].c_str() << endl;
}

// 16) Просмотр посещаемости
void viewAttendance(pqxx::work& txn) {
    cout << "\n--- Посещаемость ---" << endl;

    pqxx::result res = txn.exec(
        "SELECT a.attendance_id, "
        "s.student_code, s.full_name, "
        "COALESCE(g.name, '-') AS group_name, "
        "d.name AS discipline, sem.name AS semester, "
        "a.lesson_date, a.present "
        "FROM attendance a "
        "JOIN students s ON s.student_id = a.student_id "
        "LEFT JOIN groups g ON g.group_id = s.group_id "
        "JOIN disciplines d ON d.discipline_id = a.discipline_id "
        "JOIN semesters sem ON sem.semester_id = a.semester_id "
        "ORDER BY a.lesson_date DESC, s.student_code"
    );

    if (res.empty()) {
        cout << "Записей о посещаемости пока нет." << endl;
        return;
    }

    for (const auto& row : res) {
        bool present = row["present"].as<bool>();

        cout << "[AID " << row["attendance_id"].as<int>() << "] "
             << row["student_code"].c_str() << " | "
             << row["full_name"].c_str() << " | Группа: "
             << row["group_name"].c_str() << " | "
             << row["discipline"].c_str() << " | "
             << row["semester"].c_str() << " | "
             << row["lesson_date"].c_str() << " | "
             << (present ? "присутствовал" : "отсутствовал")
             << endl;
    }
}

// 13) Уязвимый поиск (SQLi)
void demoVulnerableSearch(pqxx::work& txn) {
    string part;
    cout << "Введите ФИО (уязвимый поиск): ";
    getline(cin, part);

    string query = "SELECT student_id, student_code, full_name FROM students "
                   "WHERE full_name ILIKE '%" + part + "%'";
    cout << "SQL: " << query << endl;

    try {
        pqxx::result res = txn.exec(query);
        for (const auto& row : res) {
            cout << row["student_code"].c_str() << " | " << row["full_name"].c_str() << endl;
        }
    } catch (const exception& e) {
        cout << "Ошибка SQL: " << e.what() << endl;
    }

    cout << "Подсказка: ' OR '1'='1" << endl;
}

// 14) UNION Injection
void demoUnionInjection(pqxx::work& txn) {
    string idStr;
    cout << "Введите ID оценки: ";
    getline(cin, idStr);

    // Важно: одинаковое число колонок и совместимые типы
    string query =
        "SELECT s.full_name, g.grade::text "
        "FROM grades g JOIN students s ON g.student_id=s.student_id "
        "WHERE g.grade_id = " + idStr;

    cout << "SQL: " << query << endl;

    try {
        pqxx::result res = txn.exec(query);
        for (const auto& row : res) {
            cout << "Данные: " << row[0].c_str() << " | " << row[1].c_str() << endl;
        }
    } catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }

    cout << "Подсказка: 1 UNION SELECT full_name, department FROM teachers" << endl;
}

// 15) Уязвимое удаление (заблокировано)
void demoVulnerableDelete(pqxx::work& txn) {
    string studentCode;
    cout << "Введите код студента для удаления посещаемости: ";
    getline(cin, studentCode);

    string query =
        "DELETE FROM attendance WHERE student_id = "
        "(SELECT student_id FROM students WHERE student_code = '" + studentCode + "')";

    cout << "SQL: " << query << endl;
    cout << "[DEMO] Выполнение заблокировано для безопасности." << endl;
    cout << "Подсказка: ' OR '1'='1" << endl;
}
