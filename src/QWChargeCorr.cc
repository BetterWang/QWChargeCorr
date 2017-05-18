/*
 * =====================================================================================
 *
 *       Filename:  QWChargeCorr.cc
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

class QWChargeCorr : public edm::EDAnalyzer {
	public:
		explicit QWChargeCorr(const edm::ParameterSet&);
		~QWChargeCorr();

	private:
		virtual void analyze(const edm::Event&, const edm::EventSetup&);


		edm::InputTag		trackEta_;
		edm::InputTag		trackPhi_;
		edm::InputTag		trackPt_;
		edm::InputTag		trackCharge_;
		edm::InputTag		trackWeight_;
		edm::InputTag		vertexZ_;

		edm::InputTag		caloArg_;
		edm::InputTag		caloAbs_;
		edm::InputTag		caloW_;

		edm::InputTag		centralityTag_;

		std::vector<double>	vCentBins_;
		std::vector<double>	vMassBins_;
		std::vector<double>	vGapBins_;

		double	minvz_, maxvz_;
		double	mineta_, maxeta_;
		double	minpt_, maxpt_;
		int	nvtx_;

		TH3D * hSS;
		TH3D * hOS;
		TH3D * hSSNP;
		TH3D * hOSNP;
};


QWChargeCorr::QWChargeCorr(const edm::ParameterSet& iConfig):
	trackEta_( iConfig.getUntrackedParameter<edm::InputTag>("trackEta") ),
	trackPhi_( iConfig.getUntrackedParameter<edm::InputTag>("trackPhi") ),
	trackPt_( iConfig.getUntrackedParameter<edm::InputTag>("trackPt") ),
	trackCharge_( iConfig.getUntrackedParameter<edm::InputTag>("trackCharge") ),
	trackWeight_( iConfig.getUntrackedParameter<edm::InputTag>("trackWeight") ),
	vertexZ_( iConfig.getUntrackedParameter<edm::InputTag>("vertexZ") ),
	caloArg_( iConfig.getUntrackedParameter<edm::InputTag>("caloArg") ),
	caloAbs_( iConfig.getUntrackedParameter<edm::InputTag>("caloAbs") ),
	caloW_( iConfig.getUntrackedParameter<edm::InputTag>("caloW") ),
	centralityTag_( iConfig.getUntrackedParameter<edm::InputTag>("centrality") ),
	vCentBins_( iConfig.getUntrackedParameter<std::vector<double> >("CentBins") ),
	vMassBins_( iConfig.getUntrackedParameter<std::vector<double> >("MassBins") ),
	vGapBins_( iConfig.getUntrackedParameter<std::vector<double> >("GapBins") )
{

        consumes<int>(centralityTag_);
        consumes<std::vector<double> >(trackEta_);
        consumes<std::vector<double> >(trackPhi_);
        consumes<std::vector<double> >(trackPt_);
        consumes<std::vector<double> >(trackWeight_);
        consumes<std::vector<double> >(trackCharge_);
        consumes<std::vector<double> >(vertexZ_);
        consumes<double >(caloArg_);
        consumes<double >(caloAbs_);
        consumes<double >(caloW_);

	minvz_ = iConfig.getUntrackedParameter<double>("minvz", -15.);
	maxvz_ = iConfig.getUntrackedParameter<double>("maxvz", 15.);

	mineta_ = iConfig.getUntrackedParameter<double>("mineta", -2.4);
	maxeta_ = iConfig.getUntrackedParameter<double>("maxeta", 2.4);
	minpt_ = iConfig.getUntrackedParameter<double>("minpt", 0.3);
	maxpt_ = iConfig.getUntrackedParameter<double>("maxpt", 3.0);

	nvtx_ = iConfig.getUntrackedParameter<int>("nvtx", 100);

	TH1D::SetDefaultSumw2();

	edm::Service<TFileService> fs;
	hSS = fs->make<TH3D>("hSS", "hSS", vMassBins_.size()-1, vMassBins_.data(), vCentBins_.size()-1, vCentBins_.data(), vGapBins_.size()-1, vGapBins_.data());
	hOS = fs->make<TH3D>("hOS", "hOS", vMassBins_.size()-1, vMassBins_.data(), vCentBins_.size()-1, vCentBins_.data(), vGapBins_.size()-1, vGapBins_.data());
	hSSNP = fs->make<TH3D>("hSSNP", "hSSNP", vMassBins_.size()-1, vMassBins_.data(), vCentBins_.size()-1, vCentBins_.data(), vGapBins_.size()-1, vGapBins_.data());
	hOSNP = fs->make<TH3D>("hOSNP", "hOSNP", vMassBins_.size()-1, vMassBins_.data(), vCentBins_.size()-1, vCentBins_.data(), vGapBins_.size()-1, vGapBins_.data());

}

QWChargeCorr::~QWChargeCorr()
{
	return;
}

void
QWChargeCorr::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	Handle<std::vector<double> >	hEta;
	Handle<std::vector<double> >	hPhi;
	Handle<std::vector<double> >	hPt;
	Handle<std::vector<double> >	hCharge;
	Handle<std::vector<double> >	hWeight;
	Handle<std::vector<double> >	hVz;
	Handle<double >	hArg;
	Handle<double >	hAbs;
	Handle<double >	hW;

	iEvent.getByLabel(trackEta_,	hEta);
	iEvent.getByLabel(trackPhi_,	hPhi);
	iEvent.getByLabel(trackPt_,	hPt);
	iEvent.getByLabel(trackCharge_, hCharge);
	iEvent.getByLabel(trackWeight_, hWeight);
	iEvent.getByLabel(vertexZ_, 	hVz);
	iEvent.getByLabel(caloArg_, 	hArg);
	iEvent.getByLabel(caloAbs_, 	hAbs);
	iEvent.getByLabel(caloW_, 	hW);

	std::complex<double> HFQ(TMath::Cos(*hArg), TMath::Sin(*hArg));
	HFQ = HFQ * (*hAbs);

	if ( hVz->size() < 1 ) return;
	if ( fabs((*hVz)[0]) > maxvz_ or fabs((*hVz)[0]) < minvz_ ) return;
	int sz = int(hEta->size());
	if ( sz == 0 ) return;

	edm::Handle<int> ch;
	iEvent.getByLabel(centralityTag_,ch);

	for ( int i = 0; i < sz; i++ ) {
		if ( (*hPt)[i] < minpt_ or (*hPt)[i] > maxpt_ ) continue;
		if ( (*hEta)[i] < mineta_ or (*hEta)[i] > maxeta_ ) continue;
		for ( int j = i+1; j < sz; j++ ) {
			if ( (*hPt)[j] < minpt_ or (*hPt)[j] > maxpt_ ) continue;
			if ( (*hEta)[j] < mineta_ or (*hEta)[j] > maxeta_ ) continue;
			double gap = fabs( (*hEta)[i] - (*hEta)[j] );

			std::complex<double> x( TMath::Cos((*hPhi)[i] + (*hPhi)[j]), TMath::Sin((*hPhi)[i] + (*hPhi)[j]) );
			x = x*std::conj(HFQ) * (*hWeight)[i] * (*hWeight)[j];
			math::PtEtaPhiMLorentzVector part1( (*hPt)[i], (*hEta)[i], (*hPhi)[i], 0.13957018 );
			math::PtEtaPhiMLorentzVector part2( (*hPt)[j], (*hEta)[j], (*hPhi)[j], 0.13957018 );
			math::PtEtaPhiMLorentzVector pair = part1 + part2;
			double minv = pair.M();
			if ( (*hCharge)[i] == (*hCharge)[j] ) {
				// SS
				hSS->Fill( minv, (*ch), gap, x.real() );
				hSSNP->Fill( minv, (*ch), gap, (*hWeight)[i] * (*hWeight)[j] * (*hW) );
			} else {
				// OS
				hOS->Fill( minv, (*ch), gap, x.real() );
				hOSNP->Fill( minv, (*ch), gap, (*hWeight)[i] * (*hWeight)[j] * (*hW) );
			}
		}
	}

	return;
}

DEFINE_FWK_MODULE(QWChargeCorr);
