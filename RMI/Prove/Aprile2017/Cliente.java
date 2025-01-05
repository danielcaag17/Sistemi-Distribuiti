package RMI.Prove.Aprile2017;

import java.rmi.Naming;

public class Cliente {
    public static void main(String[] args) {
        try {
            RemoteService1 servidor1 = (RemoteService1) Naming.lookup("//localhost/Server1");

            int[][] matriz = {
                {5, 2, 9},
                {1, 4, 6},
                {8, 3, 7}
            };

            System.out.println("Matriz original:");
            imprimirMatriz(matriz);

            int[][] matrizOrdenada = servidor1.ordenarMatriz(matriz);

            System.out.println("Matriz ordenada:");
            imprimirMatriz(matrizOrdenada);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void imprimirMatriz(int[][] matriz) {
        for (int[] fila : matriz) {
            for (int valor : fila) {
                System.out.print(valor + " ");
            }
            System.out.println();
        }
    }
}

