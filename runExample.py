# Author: John Idarraga <idarraga@cern.ch>
# ROOT macro for analysis of MediPix data using the MAFalda analysis framework
from ROOT import *
import os

# Load the Medipix analysis lib
gSystem.Load("libMediPixAnalysisCore.so")

# Load dat
mpxAnalysis = AnalysisManager("../../measurements/files.txt")

e1 = AnalysisExample;
mpxAnalysis.ConnectAlgo("SimpleExample", e1)

