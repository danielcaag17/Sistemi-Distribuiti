package RMI.Prove.Aprile2017;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RemoteService1 extends Remote {
    int[][] ordenarMatriz(int[][] matriz) throws RemoteException;
}
