import FWCore.ParameterSet.Config as cms

"""
makes a modifier config to load all the cutbased VID bits into the pat::Electron/pat::Photon userdata
any ID which starts with "mva" will not be stored
"""
def makeVIDBitsModifier(process,phoVIDModuleName):
    vidCutBitsModifier = cms.PSet(
        modifierName    = cms.string('EGExtraInfoModifierFromUIntToIntValueMaps'),
        photon_config = cms.PSet()
        )
    
    phoVIDModule = getattr(process,phoVIDModuleName)
    for egid in phoVIDModule.physicsObjectIDs:
        if egid.idDefinition.idName.value().find("mva")!=0:
            setattr(vidCutBitsModifier.photon_config,egid.idDefinition.idName.value(),cms.InputTag(phoVIDModuleName+':'+egid.idDefinition.idName.value()+"Bitmap"))

    return vidCutBitsModifier

"""
make a modifer config to load all the VID ids into the pat::Electron/pat::Photon electron/photonIDs
"""
def makeVIDinPATIDsModifier(process,phoVIDModuleName):
    vidInPATIDsModifier = cms.PSet(
        modifierName    = cms.string('EGExtraInfoModifierFromEGIDValueMaps'),
        photon_config = cms.PSet()
        )

    phoVIDModule = getattr(process,phoVIDModuleName)
    for egid in phoVIDModule.physicsObjectIDs:      
        setattr(vidInPATIDsModifier.photon_config,egid.idDefinition.idName.value(),cms.InputTag(phoVIDModuleName+':'+egid.idDefinition.idName.value()))
    
    return vidInPATIDsModifier
    
def makeEnergyScaleAndSmearingSysModifier(phoProdName):
    energyScaleAndSmearing = cms.PSet(
        modifierName    = cms.string('EGExtraInfoModifierFromFloatValueMaps'),
        photon_config = cms.PSet()
        )

    from RecoEgamma.EgammaTools.calibratedEgammas_cff import prefixName
    import RecoEgamma.EgammaTools.calibratedPhotonProducerTRecoPhoton_cfi
    for valueMapName in RecoEgamma.EgammaTools.calibratedPhotonProducerTRecoPhoton_cfi.calibratedPhotonProducerTRecoPhoton.valueMapsStored:
        setattr(energyScaleAndSmearing.photon_config,valueMapName,cms.InputTag(phoProdName,valueMapName))
    return energyScaleAndSmearing
