import FWCore.ParameterSet.Config as cms
from RecoEgamma.EgammaPhotonProducers.gedPhotons_cfi import *

ootPhotons = gedPhotons.clone()
ootPhotons.photonProducer = cms.InputTag("ootPhotonCore")
ootPhotons.candidateP4type = cms.string("fromEcalEnergy")
del ootPhotons.regressionConfig
ootPhotons.outputPhotonCollection = cms.string("")
ootPhotons.reconstructionStep = cms.string("oot")



print ootPhotons.outputPhotonCollection

#ootPhotons.pfEgammaCandidates = cms.InputTag("ba")
#ootPhotons.pfCandidates = cms.InputTag("ba")
#ootPhotons.valueMapPhotons = cms.string("valMapOOTPhotons")

print 'here'
