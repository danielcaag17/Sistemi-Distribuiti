package RMI.Prove.Aprile2017;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RemoteService2 extends Remote {
    int[] encontrarMinimo(int[] vector) throws RemoteException;
}

