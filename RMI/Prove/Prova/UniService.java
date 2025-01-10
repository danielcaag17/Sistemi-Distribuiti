package RMI.Prove.Prova;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface UniService extends Remote{
    public String register(String name, String surname, String matriculationNumber, int courseCode) throws RemoteException;
    public void deleteRegistrarion(String id) throws RemoteException;
    public int howManyRegistered(int courseCode) throws RemoteException;
}