import java.util.Random;
import java.util.TreeSet;

class GenerateRandom{



    public static int generateNumberOnRange(int min,int max){
        return new Random().nextInt((max-min) + 1)+min;
    }

    public static TreeSet<Integer> generateSetOnRange(int range){
        TreeSet<Integer>generatedTreeSet = new TreeSet<>();

        for(int i = 0; i < range;i++){
            generatedTreeSet.add(generateNumberOnRange(20,2000000));
        }
        return generatedTreeSet;
    }

}

class acessVectorOnRange implements Runnable{
    private int start;
    private int end;
    private TreeSet<Integer>[] vec;
    public acessVectorOnRange(int start, int end, TreeSet<Integer>[]vec){
        this.start = start;
        this.end = end;
        this.vec = vec;
    }

    @Override
    public void run(){
        for(int i = start; i < end;i++){
            System.out.println(i);
            vec[i] = GenerateRandom.generateSetOnRange(1000000);
        }
    }
}

public class Main {
    public static void main(String[] args) throws InterruptedException {
        final int numThreads = 4;

        int quantityLine = 40;//GenerateRandom.generateNumberOnRange(5,20);

        TreeSet<Integer>[] mapVector = new TreeSet[quantityLine];
        Thread[] threadsLine = new Thread[numThreads];

        int start;
        int end;
        int chunkByThread = quantityLine/numThreads;

        System.out.println("Tamanho: " + quantityLine);
        long beginMultiThread = System.currentTimeMillis();

        for(int i = 0; i < numThreads;i++){
            start = i*chunkByThread;

            if(i == numThreads-1) {
                end = quantityLine;
            }
            else {
                end = ((i + 1) * chunkByThread);
            }
            threadsLine[i] = new Thread(new acessVectorOnRange(start,end,mapVector));
            threadsLine[i].start();
        }

        for(int i = 0;i < numThreads;i++){
            System.out.println("Thread: "+ threadsLine[i].getName() + " finalizada");
            threadsLine[i].join();
        }

        long endMultiThread = System.currentTimeMillis();
        long elapsedMultiThread = endMultiThread-beginMultiThread;
        System.out.println("Tempo de execução multithread: " + elapsedMultiThread + " ms");

    }
}