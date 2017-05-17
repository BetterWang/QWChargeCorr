from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

config = config()

config.General.requestName = 'PAHM0_ChargeCorr_eff_v1'
config.General.workArea = 'CrabArea'
config.General.transferOutputs = True
config.General.transferLogs = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'qwcumu_pPb16_HM_eff_v1.py'
config.Data.inputDataset = '/PAHighMultiplicity0/PARun2016C-PromptReco-v1/AOD'
config.JobType.inputFiles = ['Hijing_8TeV_dataBS.root']
#config.Data.inputDBS = 'phys03'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
config.Data.outLFNDirBase = '/store/group/phys_heavyions/qwang/cumu/'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/HI/Cert_285479-285832_HI8TeV_PromptReco_pPb_Collisions16_JSON_NoL1T.txt'
config.Data.publication = False
config.Data.useParent = False
config.Site.storageSite = 'T2_CH_CERN'
config.Site.ignoreGlobalBlacklist = True
#config.Data.allowNonValidInputDataset = True
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)



### 1
config.Data.inputDataset = '/PAHighMultiplicity1/PARun2016C-PromptReco-v1/AOD'

config.General.requestName = 'PAHM1_ChargeCorr_eff_v1'
config.JobType.psetName = 'qwcumu_pPb16_HM1_eff_v1.py'
config.JobType.inputFiles = ['Hijing_8TeV_dataBS.root']
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)


## 7
config.Data.inputDataset = '/PAHighMultiplicity7/PARun2016C-PromptReco-v1/AOD'

config.General.requestName = 'PAHM7_ChargeCorr_eff_v1'
config.JobType.psetName = 'qwcumu_pPb16_HM7_eff_v1.py'
config.JobType.inputFiles = ['Hijing_8TeV_dataBS.root']
try:
        crabCommand('submit', config = config)
except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
except ClientException as cle:
        print "Failed submitting task: %s" % (cle)


