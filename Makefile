pinyin: bin/predict.o bin/model.o bin/main.o
	g++ -std=c++11 -o bin/pinyin bin/main.o bin/predict.o bin/model.o

bin/main.o: src/main.cpp bin/predict.o
	g++ -std=c++11 -c src/main.cpp -o bin/main.o

bin/predict.o: src/predict.cpp src/predict.h
	g++ -std=c++11 -c src/predict.cpp -o bin/predict.o

bin/model.o: src/model.cpp src/model.h
	g++ -std=c++11 -c src/model.cpp -o bin/model.o

clean:
	rm -rf bin/*.o bin/predict
