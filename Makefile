all: proteins

proteins:
	mkdir -p ./build ./results
	
	g++ -o ./build/proteins_clustering ./src/temp_main.cpp ./src/main_functions.cpp ./src/Object_Info.cpp \
	./src/Distance.cpp ./src/Transform.cpp ./src/Initialization.cpp ./src/Cluster.cpp \
	./src/Update.cpp ./src/Assignment.cpp ./src/Silhouette.cpp -I ./include/Eigen

roads:
	@echo "nothing yet"

clean:
	rm -rf ./build/* ./results/*.dat