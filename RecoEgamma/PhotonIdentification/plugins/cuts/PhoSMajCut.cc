#include "PhysicsTools/SelectorUtils/interface/CutApplicatorWithEventContentBase.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"


class PhoSMajCut : public CutApplicatorWithEventContentBase {
public:
  PhoSMajCut(const edm::ParameterSet& c);
  
  result_type operator()(const reco::PhotonPtr&) const final;

  void setConsumes(edm::ConsumesCollector&) final;
  void getEventContent(const edm::EventBase&) final;

  double value(const reco::CandidatePtr& cand) const final;

  CandidateType candidateType() const final { 
    return PHOTON; 
  }

private:
  // Cut values
  float _C1_EB;
  float _C1_EE;

  // Configuration
  float _barrelCutOff;

  // The smaj computed upstream
  edm::Handle<edm::ValueMap<float> > _smajMap;
  // The rho
  constexpr static char smaj_[] = "sMaj";

};

constexpr char PhoSMajCut::smaj_[];


DEFINE_EDM_PLUGIN(CutApplicatorFactory,
		  PhoSMajCut,
		  "PhoSMajCut");

PhoSMajCut::PhoSMajCut(const edm::ParameterSet& c) :
  CutApplicatorWithEventContentBase(c),
  _C1_EB(c.getParameter<double>("cutValueEB")),
  _C1_EE(c.getParameter<double>("cutValueEE")),
  _barrelCutOff(c.getParameter<double>("barrelCutOff"))
{
  
  edm::InputTag maptag = c.getParameter<edm::InputTag>("smajMap");
  contentTags_.emplace(smaj_,maptag);

}

void PhoSMajCut::setConsumes(edm::ConsumesCollector& cc) {
  auto smaj =  cc.consumes<edm::ValueMap<float> >(contentTags_[smaj_]);
  contentTokens_.emplace(smaj_,smaj);

}

void PhoSMajCut::getEventContent(const edm::EventBase& ev) {  
  ev.getByLabel(contentTags_[smaj_],_smajMap);

}

CutApplicatorBase::result_type 
PhoSMajCut::
operator()(const reco::PhotonPtr& cand) const{  

  // in case we are by-value
  const std::string& inst_name = contentTags_.find(smaj_)->second.instance();
  edm::Ptr<pat::Photon> pat(cand);
  float smajval = -1.0;
  std::cout<<"smaj is valid "<<_smajMap.isValid()<<std::endl;
  std::cout<<"_smajMap->contains( cand.id() "<<_smajMap->contains( cand.id())<<std::endl;  
  std::cout<<" _smajMap->idSize() " <<_smajMap->idSize() <<std::endl;
  std::cout<< "(*_smajMap)[cand] "<< (*_smajMap)[cand] <<std::endl;
   if( _smajMap.isValid() && _smajMap->contains( cand.id() ) ) {
    smajval = (*_smajMap)[cand];
  } else if ( _smajMap.isValid() && _smajMap->idSize() == 1 &&
              cand.id() == edm::ProductID() ) {
    // in case we have spoofed a ptr
    //note this must be a 1:1 valuemap (only one product input)
    smajval = _smajMap->begin()[cand.key()];
  } else if ( _smajMap.isValid() ){ // throw an exception
    smajval = (*_smajMap)[cand];
  }

  // Figure out the cut value
  // The value is generally pt-dependent: C1 + pt * C2
  const double absEta = std::abs(cand->superCluster()->eta());
  const float smajCutValue = 
    ( absEta < _barrelCutOff ? 
      _C1_EB 
      : 
      _C1_EE 
      );
  
  // Retrieve the variable value for this particle
  //  float smaj = _smajMap.isValid() ? smajval : pat->userFloat(inst_name);

  // Apply the cut and return the result
  // Scale by pT if the relative isolation is requested but avoid division by 0
  std::cout<<smajval<<std::endl;
  return smajval < smajCutValue;
}


double PhoSMajCut::
value(const reco::CandidatePtr& cand) const {
  reco::PhotonPtr pho(cand);

  // in case we are by-value
  const std::string& inst_name = contentTags_.find(smaj_)->second.instance();
  edm::Ptr<pat::Photon> pat(cand);
  float smajval = -1.0;
  if( _smajMap.isValid() && _smajMap->contains( cand.id() ) ) {
    smajval = (*_smajMap)[cand];
  } else if ( _smajMap.isValid() && _smajMap->idSize() == 1 &&
              cand.id() == edm::ProductID() ) {
    // in case we have spoofed a ptr
    //note this must be a 1:1 valuemap (only one product input)
    smajval = _smajMap->begin()[cand.key()];
  } else if ( _smajMap.isValid() ){ // throw an exception
    smajval = (*_smajMap)[cand];
  }

  // Figure out the cut value
  // The value is generally pt-dependent: C1 + pt * C2
  double absEta = std::abs(pho->superCluster()->eta());  
  
  // Retrieve the variable value for this particle
  //  float smaj = _smajMap.isValid() ? smajval : pat->userFloat(inst_name);

  // Apply the cut and return the result
  return smajval;
}
