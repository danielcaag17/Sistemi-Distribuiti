package RMI.Prove.Maggio2011;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface WriteService  extends Remote{
    public void writeFile(String Path, String s) throws RemoteException;
}