CC=g++
OPTS=-g

all: main.o traceread.o predictor.o PAgA2.o Perceptron.o GShare.o Alpha.o
	$(CC) $(OPTS) -lm -o predictor main.o traceread.o predictor.o PAgA2.o Perceptron.o GShare.o Alpha.o

main.o: main.C traceread.h predictor.h
	$(CC) $(OPTS) -c main.C

traceread.o: traceread.h traceread.C
	$(CC) $(OPTS) -c traceread.C

predictor.o: predictor.h predictor.C
	$(CC) $(OPTS) -c predictor.C

PAgA2.o: PAgA2.h Predictor_base.h PAgA2.C
	$(CC) $(OPTS) -c PAgA2.C

Perceptron.o: Perceptron.h Predictor_base.h Perceptron.C
	$(CC) $(OPTS) -c Perceptron.C

GShare.o: GShare.h Predictor_base.h GShare.C
	$(CC) $(OPTS) -c GShare.C

Alpha.o: Alpha.h Predictor_base.h Alpha.C
	$(CC) $(OPTS) -c Alpha.C

clean:
	rm -f *.o predictor;
