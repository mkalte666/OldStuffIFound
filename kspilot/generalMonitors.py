from FlightManager import Task
import os


def cls():
    os.system('cls' if os.name=='nt' else 'clear')

class DataMonitor( Task ):
    def tick(self, manager):
        cls()
        print("Altitude: "+str(manager.vessel.telemetry.altitude()))