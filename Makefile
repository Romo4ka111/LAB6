CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS =
TARGET = student_app
SOURCES = main.cpp operations.cpp
HEADERS = operations.h

# Флаги от pkg-config
PKG_CFLAGS = $(shell pkg-config --cflags libpqxx)
PKG_LIBS   = $(shell pkg-config --libs libpqxx libpq)

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(PKG_CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS) $(PKG_LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

db-init:
	psql -U postgres -d students_db -f create_tables.sql
	psql -U postgres -d students_db -f insert_data.sql

.PHONY: all clean run db-init
##