# Перечень всех исходных файлов
SRCS = main.cpp streebog.cpp streebog_sse41.cpp crypto_hash.cpp cpuinfo.cpp

# Генерация соответствующих объектных файлов
OBJS = $(SRCS:.cpp=.o)

# Настройки компилятора
CXX = g++
CXXFLAGS = -Wall -std=c++11 -msse4.1

# Целевой исполняемый файл
TARGET = streebog_hash

# Сборка целевого файла
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Компиляция исходных файлов в объектные файлы
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Очистка артефактов сборки
clean:
	rm -f $(OBJS) $(TARGET)
