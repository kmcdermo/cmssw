import FWCore.ParameterSet.Config as cms
from RecoEgamma.EgammaPhotonProducers.gedPhotons_cfi import gedPhotons as _gedPhotons

ootPhotons = _gedPhotons.clone(
    photonProducer = 'ootPhotonCore',
    candidateP4type = "fromEcalEnergy",
    reconstructionStep = "oot",
    pfEgammaCandidates = "",
    valueMapPhotons = ""
    )
del ootPhotons.regressionConfig

from Configuration.Eras.Modifier_run2_miniAOD_80XLegacy_cff import run2_miniAOD_80XLegacy

run2_miniAOD_80XLegacy.toModify(ootPhotons, barrelEcalHits = "reducedEcalRecHitsEB")
run2_miniAOD_80XLegacy.toModify(ootPhotons, endcapEcalHits = "reducedEcalRecHitsEE")
run2_miniAOD_80XLegacy.toModify(ootPhotons, preshowerHits = "reducedEcalRecHitsES")
run2_miniAOD_80XLegacy.toModify(ootPhotons, hcalTowers = "")

