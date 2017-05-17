/*
 * =====================================================================================
 *
 *       Filename:  QWQProduct.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/16/2017 15:22:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Quan Wang 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Math/interface/LorentzVector.h"


#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TNtuple.h"
#include "TComplex.h"
#include <complex>

using namespace std;

class QWQProduct : public edm::EDAnalyzer {
	public:
		explicit QWQProduct(const edm::ParameterSet&);
		~QWQProduct();

	private:
		virtual void analyze(const edm::Event&, const edm::EventSetup&);


		edm::InputTag		caloArgP_;
		edm::InputTag		caloAbsP_;
		edm::InputTag		caloWP_;

		edm::InputTag		caloArgM_;
		edm::InputTag		caloAbsM_;
		edm::InputTag		caloWM_;

		edm::InputTag		trkArg_;
		edm::InputTag		trkAbs_;
		edm::InputTag		trkW_;

		edm::InputTag		centralityTag_;

		std::vector<double>	vCentBins_;

		TH1D *	hHFpHFm;
		TH1D *	hHFpTrk;
		TH1D *	hHFmTrk;
		TH1D *	hwHFpHFm;
		TH1D *	hwHFpTrk;
		TH1D *	hwHFmTrk;
};


QWQProduct::QWQProduct(const edm::ParameterSet& iConfig):
	caloArgP_( iConfig.getUntrackedParameter<edm::InputTag>("caloArgP") ),
	caloAbsP_( iConfig.getUntrackedParameter<edm::InputTag>("caloAbsP") ),
	caloWP_( iConfig.getUntrackedParameter<edm::InputTag>("caloWP") ),
	caloArgM_( iConfig.getUntrackedParameter<edm::InputTag>("caloArgM") ),
	caloAbsM_( iConfig.getUntrackedParameter<edm::InputTag>("caloAbsM") ),
	caloWM_( iConfig.getUntrackedParameter<edm::InputTag>("caloWM") ),
	trkArg_( iConfig.getUntrackedParameter<edm::InputTag>("trkArg") ),
	trkAbs_( iConfig.getUntrackedParameter<edm::InputTag>("trkAbs") ),
	trkW_( iConfig.getUntrackedParameter<edm::InputTag>("trkW") ),
	centralityTag_( iConfig.getUntrackedParameter<edm::InputTag>("centrality") ),
	vCentBins_( iConfig.getUntrackedParameter<std::vector<double> >("CentBins") )
{

        consumes<int>(centralityTag_);
        consumes<double >(caloArgP_);
        consumes<double >(caloAbsP_);
        consumes<double >(caloWP_);
        consumes<double >(caloArgM_);
        consumes<double >(caloAbsM_);
        consumes<double >(caloWM_);
        consumes<double >(trkArg_);
        consumes<double >(trkAbs_);
        consumes<double >(trkW_);


	TH1D::SetDefaultSumw2();

	edm::Service<TFileService> fs;
	hHFpHFm = fs->make<TH1D>("hHFpHFm", "hHFpHFm;Cent", vCentBins_.size()-1, vCentBins_.data());
	hHFpTrk = fs->make<TH1D>("hHFpTrk", "hHFpTrk;Cent", vCentBins_.size()-1, vCentBins_.data());
	hHFmTrk = fs->make<TH1D>("hHFmTrk", "hHFmTrk;Cent", vCentBins_.size()-1, vCentBins_.data());
	hwHFpHFm = fs->make<TH1D>("hwHFpHFm", "hwHFpHFm;Cent", vCentBins_.size()-1, vCentBins_.data());
	hwHFpTrk = fs->make<TH1D>("hwHFpTrk", "hwHFpTrk;Cent", vCentBins_.size()-1, vCentBins_.data());
	hwHFmTrk = fs->make<TH1D>("hwHFmTrk", "hwHFmTrk;Cent", vCentBins_.size()-1, vCentBins_.data());
}

QWQProduct::~QWQProduct()
{
	return;
}

void
QWQProduct::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	Handle<double >	hcaloArgP;
	Handle<double >	hcaloArgM;
	Handle<double >	hcaloAbsP;
	Handle<double >	hcaloAbsM;
	Handle<double >	hcaloWP;
	Handle<double >	hcaloWM;

	Handle<double >	htrkArg;
	Handle<double >	htrkAbs;
	Handle<double >	htrkW;

	iEvent.getByLabel(caloArgP_,	hcaloArgP);
	iEvent.getByLabel(caloArgM_,	hcaloArgM);
	iEvent.getByLabel(caloAbsP_,	hcaloAbsP);
	iEvent.getByLabel(caloAbsM_,	hcaloAbsM);
	iEvent.getByLabel(caloWP_  ,	hcaloWP  );
	iEvent.getByLabel(caloWM_  ,	hcaloWM  );

	iEvent.getByLabel(trkArg_,	htrkArg);
	iEvent.getByLabel(trkAbs_,	htrkAbs);
	iEvent.getByLabel(trkW_,	htrkW);

	edm::Handle<int> ch;
	iEvent.getByLabel(centralityTag_,ch);

	std::complex<double>	QHFp(TMath::Cos(*hcaloArgP), TMath::Sin(*hcaloArgP));
	std::complex<double>	QHFm(TMath::Cos(*hcaloArgM), TMath::Sin(*hcaloArgM));
	std::complex<double>	Qtrk(TMath::Cos(*htrkArg), TMath::Sin(*htrkArg));
	QHFp = QHFp * hcaloAbsP;
	QHFm = QHFm * hcaloAbsM;
	Qtrk = Qtrk * htrkAbs;

	hHFpHFm->Fill( *ch, (QHFp * std::conj(QHFm)).real() );
	hHFpTrk->Fill( *ch, (QHFp * std::conj(Qtrk)).real() );
	hHFmTrk->Fill( *ch, (QHFm * std::conj(Qtrk)).real() );

	hwHFpHFm->Fill( *ch, (*hcaloWP)*(*hcaloWM) );
	hwHFpTrk->Fill( *ch, (*hcaloWP)*(*htrkW) );
	hwHFmTrk->Fill( *ch, (*hcaloWM)*(*htrkW) );

	return;
}

DEFINE_FWK_MODULE(QWQProduct);
