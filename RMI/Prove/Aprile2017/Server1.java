package RMI.Prove.Aprile2017;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class Server1 extends UnicastRemoteObject implements RemoteService1 {

    private RemoteService2 servidor2;

    public Server1(RemoteService2 servidor2) throws RemoteException {
        super();
        this.servidor2 = servidor2;
    }

    @Override
    public int[][] ordenarMatriz(int[][] matriz) throws RemoteException {
        int filas = matriz.length;
        int columnas = matriz[0].length;

        for (int i = 0; i < filas - 1; i++) {
            for (int j = i + 1; j < filas; j++) {
                int[] minimoInfo = servidor2.encontrarMinimo(matriz[j]);
                int valorMinimo = minimoInfo[0];
                if (valorMinimo < matriz[i][columnas - 1]) {
                    int[] temp = matriz[i];
                    matriz[i] = matriz[j];
                    matriz[j] = temp;
                }
            }
        }
        return matriz;
    }

    public static void main(String[] args) {
        try {
            RemoteService2 server2 = (RemoteService2) Naming.lookup("//localhost/Server2");
            Server1 server1 = new Server1(server2);
            Naming.rebind("//localhost/Server1", server1);
            System.out.println("Server1 listo.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
