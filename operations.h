#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>

using namespace std;

struct DbConfig {
    string connectionString;
};

void showMenu();

void viewAllFlights(pqxx::work& txn);   // теперь: все оценки
void viewAllAirports(pqxx::work& txn);  // теперь: все студенты
void viewAllPlanes(pqxx::work& txn);    // теперь: все дисциплины

void searchFlightsByCity(pqxx::work& txn);      // теперь: поиск оценок по студенту
void searchPlanesByAirline(pqxx::work& txn);    // теперь: дисциплины по преподавателю

void registerNewPassenger(pqxx::work& txn);     // теперь: добавить оценку
void updateFlightStatus(pqxx::work& txn);       // теперь: изменить оценку
void cancelFlight(pqxx::work& txn);             // теперь: удалить оценку

void showAirportsStats(pqxx::work& txn);        // аналитика по группам
void showMostExpensiveFlight(pqxx::work& txn);  // максимум оценка
void showAirlinesFleet(pqxx::work& txn);        // нагрузка преподавателей
void showGeneralStats(pqxx::work& txn);         // общая статистика

void demoVulnerableSearch(pqxx::work& txn);
void demoUnionInjection(pqxx::work& txn);
void demoVulnerableDelete(pqxx::work& txn);

// 16) Просмотр посещаемости
void viewAttendance(pqxx::work& txn);

#endif
//