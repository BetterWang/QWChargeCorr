import FWCore.ParameterSet.Config as cms

process = cms.Process("ChargeCorr")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')


process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/q/qwang/work/cleanroomRun2/Ana/data/pPb_8_FEVT.root")
)

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltMB = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMB.HLTPaths = [
		        "HLT_PAL1MinimumBiasHF_OR_SinglePixelTrack_part*",
			]
process.hltMB.andOr = cms.bool(True)
process.hltMB.throw = cms.bool(False)


process.chargecorrP = cms.EDAnalyzer('QWChargeCorr'
	, trackEta = cms.untracked.InputTag('QWEvent', "eta")
	, trackPhi = cms.untracked.InputTag('QWEvent', "phi")
	, trackPt = cms.untracked.InputTag('QWEvent', "pt")
	, trackWeight = cms.untracked.InputTag('QWEvent', "weight")
	, trackCharge = cms.untracked.InputTag('QWEvent', "charge")
	, vertexZ = cms.untracked.InputTag('QWEvent', "vz")
	, caloArg = cms.untracked.InputTag('HFQvect', 'argp')
	, caloAbs = cms.untracked.InputTag('HFQvect', 'absp')
	, caloW = cms.untracked.InputTag('HFQvect', 'Wplus')
	, centrality = cms.untracked.InputTag('Noff')
	, minvz = cms.untracked.double(-15.0)
	, maxvz = cms.untracked.double(15.0)
	, mineta = cms.untracked.double(-2.4)
	, maxeta = cms.untracked.double(2.4)
	, minpt = cms.untracked.double(0.3)
	, maxpt = cms.untracked.double(3.0)
	, nvtx = cms.untracked.int32(100)
	, CentBins = cms.untracked.vdouble(0, 10, 30, 50, 70, 90, 120, 150, 185, 220, 250, 300, 320, 350, 600)
	, MassBins = cms.untracked.vdouble(0.1 ,  0.12,  0.14,  0.16,  0.18,  0.2 ,  0.22,  0.24,  0.26, 0.28,  0.3 ,  0.32,  0.34,  0.36,  0.38,  0.4 ,  0.42,  0.44,
		0.46,  0.48,  0.5 ,  0.52,  0.54,  0.56,  0.58,  0.6 ,  0.62, 0.64,  0.66,  0.68,  0.7 ,  0.72,  0.74,  0.76,  0.78,  0.8 ,
		0.82,  0.84,  0.86,  0.88,  0.9 ,  0.92,  0.94,  0.96,  0.98, 1. ,  1.1,  1.2,  1.3,  1.4,  1.5,  1.6,  1.7,  1.8,  1.9,  2. ,
		2.1,  2.2,  2.3,  2.4,  2.5,  2.6,  2.7,  2.8,  2.9,  3.)
	, GapBins = cms.untracked.vdouble(0. ,  0.3,  0.6,  0.9,  1.2,  1.5,  1.8,  2.1,  2.4,  2.7,  3. , 3.3,  3.6,  3.9,  4.2,  4.5, 4.8)
)

process.chargecorrM = process.chargecorrP.clone(
	caloArg = cms.untracked.InputTag('HFQvect', 'argm'),
	caloAbs = cms.untracked.InputTag('HFQvect', 'absm'),
	caloW = cms.untracked.InputTag('HFQvect', 'Wminus')
        )

process.qprod = cms.EDAnalyzer('QWQProduct'
        , caloArgP = cms.untracked.InputTag('HFQvect', 'argp')
        , caloAbsP = cms.untracked.InputTag('HFQvect', 'absp')
        , caloWP = cms.untracked.InputTag('HFQvect', 'Wplus')
        , caloArgM = cms.untracked.InputTag('HFQvect', 'argm')
        , caloAbsM = cms.untracked.InputTag('HFQvect', 'absm')
        , caloWM = cms.untracked.InputTag('HFQvect', 'Wminus')
        , trkArg = cms.untracked.InputTag('TrkQvect', 'arg')
        , trkAbs = cms.untracked.InputTag('TrkQvect', 'abs')
        , trkW = cms.untracked.InputTag('TrkQvect', 'W')
	, centrality = cms.untracked.InputTag('Noff')
	, CentBins = cms.untracked.vdouble(0, 10, 30, 50, 70, 90, 120, 150, 185, 220, 250, 300, 320, 350, 600)
        )

process.TrkQvect = cms.EDProducer('QWTrackQProducer'
	, trackEta = cms.untracked.InputTag('QWEvent', "eta")
	, trackPhi = cms.untracked.InputTag('QWEvent', "phi")
	, trackPt = cms.untracked.InputTag('QWEvent', "pt")
	, trackWeight = cms.untracked.InputTag('QWEvent', "weight")
        , etaMin = cms.untracked.double(-2.4)
        , etaMax = cms.untracked.double(2.4)
        , ptMin = cms.untracked.double(0.3)
        , ptMax = cms.untracked.double(3.0)
	, N = cms.untracked.int32(2)
	, ptWeight = cms.untracked.bool(True)
        )

process.HFQvect = cms.EDProducer('QWCaloQProducer'
		, caloSrc = cms.untracked.InputTag('towerMaker')
		, etaMin = cms.untracked.double(3.)
		, etaMax = cms.untracked.double(5.)
		, N = cms.untracked.int32(2)
		)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('cumu.root')
)


process.load("HeavyIonsAnalysis.VertexAnalysis.PAPileUpVertexFilter_cff")

process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2 && tracksSize >= 2"),
    filter = cms.bool(True), # otherwise it won't filter the events
)

process.NoScraping = cms.EDFilter("FilterOutScraping",
 applyfilter = cms.untracked.bool(True),
 debugOn = cms.untracked.bool(False),
 numtrack = cms.untracked.uint32(10),
 thresh = cms.untracked.double(0.25)
)

process.load("HeavyIonsAnalysis.Configuration.hfCoincFilter_cff")
process.load("HeavyIonsAnalysis.EventAnalysis.pileUpFilter_cff")

process.eventSelection = cms.Sequence(process.hfCoincFilter * process.PAprimaryVertexFilter * process.NoScraping * process.olvFilter_pPb8TeV_dz1p0)

process.load('RecoHI.HiCentralityAlgos.CentralityFilter_cfi')
process.ppNoffFilter120 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(120, 150)
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter150 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(150, 185)
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter185 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(185, 250)
			),
		BinLabel = cms.InputTag("Noff")
		)

process.ppNoffFilter250 = process.centralityFilter.clone(
		selectedBins = cms.vint32(
			*range(250, 600)
			),
		BinLabel = cms.InputTag("Noff")
		)

process.load('pPb_HM_eff')
process.QWEvent.fweight = cms.untracked.InputTag('Hijing_8TeV_dataBS.root')

process.histHFPhiSum = cms.EDAnalyzer('QWHistDAnalyzer',
		src = cms.untracked.InputTag('HFQvect', 'arg'),
		Nbins = cms.untracked.int32(1000),
		start = cms.untracked.double(-5),
		end = cms.untracked.double(5),
		)

process.histHFPhiPlus = process.histHFPhiSum.clone(
		src = cms.untracked.InputTag('HFQvect', 'argp')
		)

process.histHFPhiMinus = process.histHFPhiSum.clone(
		src = cms.untracked.InputTag('HFQvect', 'argm')
		)

process.HFmon = cms.Sequence(process.histHFPhiSum + process.histHFPhiPlus + process.histHFPhiMinus)

process.mon = cms.Sequence(process.histNoff + process.vectPhi + process.vectPt + process.vectEta + process.vectPhiW + process.vectPtW + process.vectEtaW + process.HFmon )

process.ana = cms.Path(process.hltMB*process.eventSelection*process.Noff*process.QWEvent * process.TrkQvect * process.HFQvect * process.chargecorrP * process.chargecorrM * process.qprod * process.mon)

process.schedule = cms.Schedule(
	process.ana
)
