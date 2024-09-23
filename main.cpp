#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <vector>
#include <string>

class Calendar {
public:
    Calendar() {
        time_t t = time(0);
        struct tm* now = localtime(&t);
        currentYear = now->tm_year + 1900;
        currentMonth = now->tm_mon + 1;
        currentDay = now->tm_mday;
    }

    void draw(sf::RenderWindow& window) {
        window.clear(sf::Color::White);

        // Draw month and year
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error loading font\n";
        }

        sf::Text monthYearText;
        monthYearText.setFont(font);
        monthYearText.setString(getMonthName(currentMonth) + " " + std::to_string(currentYear));
        monthYearText.setCharacterSize(24);
        monthYearText.setFillColor(sf::Color::Black);
        monthYearText.setPosition(10, 10);
        window.draw(monthYearText);

        // Draw days of the week
        std::vector<std::string> daysOfWeek = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
        for (int i = 0; i < 7; ++i) {
            sf::Text dayText;
            dayText.setFont(font);
            dayText.setString(daysOfWeek[i]);
            dayText.setCharacterSize(20);
            dayText.setFillColor(sf::Color::Black);
            dayText.setPosition(10 + i * 50, 50);
            window.draw(dayText);
        }

        // Draw days of the month
        int daysInMonth = getDaysInMonth(currentMonth, currentYear);
        int startDay = getStartDay(currentMonth, currentYear);
        for (int i = 1; i <= daysInMonth; ++i) {
            sf::Text dayText;
            dayText.setFont(font);
            dayText.setString(std::to_string(i));
            dayText.setCharacterSize(20);
            dayText.setFillColor(sf::Color::Black);
            int x = (startDay + i - 1) % 7;
            int y = (startDay + i - 1) / 7;
            dayText.setPosition(10 + x * 50, 80 + y * 30);
            window.draw(dayText);
        }

        window.display();
    }

    void nextMonth() {
        if (currentMonth == 12) {
            currentMonth = 1;
            ++currentYear;
        } else {
            ++currentMonth;
        }
    }

    void previousMonth() {
        if (currentMonth == 1) {
            currentMonth = 12;
            --currentYear;
        } else {
            --currentMonth;
        }
    }

private:
    int currentYear, currentMonth, currentDay;

    std::string getMonthName(int month) {
        std::vector<std::string> months = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
        return months[month - 1];
    }

    int getDaysInMonth(int month, int year) {
        if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                return 29;
            } else {
                return 28;
            }
        } else if (month == 4 || month == 6 || month == 9 || month == 11) {
            return 30;
        } else {
            return 31;
        }
    }

    int getStartDay(int month, int year) {
        struct tm time_in = { 0, 0, 0, 1, month - 1, year - 1900 };
        time_t time_temp = mktime(&time_in);
        const struct tm* time_out = localtime(&time_temp);
        return time_out->tm_wday;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(400, 300), "Calendar");

    Calendar calendar;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Right) {
                    calendar.nextMonth();
                } else if (event.key.code == sf::Keyboard::Left) {
                    calendar.previousMonth();
                }
            }
        }

        calendar.draw(window);
    }

    return 0;
}