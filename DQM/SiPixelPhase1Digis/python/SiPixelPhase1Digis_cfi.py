import FWCore.ParameterSet.Config as cms

# this might also go into te Common config,as we do not reference it
from DQM.SiPixelPhase1Common.HistogramManager_cfi import *

SiPixelPhase1DigisADC = DefaultHisto.clone(
  enabled = True,
  name = "adc",
  title = "Digi ADC values",
  xlabel = "adc readout",
  range_min = 0,
  range_max = 300,
  range_nbins = 300,
  specs = cms.VPSet(
    Specification().groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|/PXLadder|PXBlade/PXBModule|PXPanel")
                   .save()
                   .reduce("MEAN")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|", "EXTEND_X")
                   .saveAll(),
    Specification().groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|/PXLadder|PXBlade")
                   .save()
                   .reduce("MEAN")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|", "SUM")
                   .saveAll(),
    Specification().groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|/signedLadder|PXBlade/signedModule|PXPanel")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|/signedLadder|PXBlade", "EXTEND_X")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|", "EXTEND_Y")
                   .reduce("MEAN")
                   .save()
    #StandardSpecificationTrend,
    #StandardSpecification2DProfile,
    #*StandardSpecifications1D
  )
)

SiPixelPhase1DigisNdigis = DefaultHisto.clone(
  enabled = True,
  name = "digis", # 'Count of' added automatically
  title = "Digis",
  xlabel = "digis",
  range_min = 0,
  range_max = 30,
  range_nbins = 30,
  dimensions = 0, # this is a count
  specs = cms.VPSet(
    Specification().groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|/signedLadder|PXBlade/signedModule|PXPanel")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|/signedLadder|PXBlade", "EXTEND_X")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|", "EXTEND_Y")
                   .save()
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk")
                   .saveAll(),
    Specification().groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|/PXBModule|PXPanel")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/PXRing|", "EXTEND_X")
                   .save()
                   .groupBy("PXBarrel|PXForward/PXDisk|/PXRing|", "SUM")
                   .save(),
    Specification().groupBy("signedModule|PXDisk/PXRing|PXLayer/PXPanel|/Event")
                   .reduce("COUNT")
                   .groupBy("PXBarrel|PXForward/PXLayer|/signedModule|PXDisk")
                   .groupBy("PXBarrel|PXForward/PXLayer|", "EXTEND_X")
                   .reduce("MEAN")
                   .save()
                   .groupBy("PXBarrel|PXForward", "SUM")
                   .save(),
    Specification().groupBy("PXBarrel|PXForward/PXLayer|/signedModule|PXDisk")
                   .groupBy("PXBarrel|PXForward/PXLayer|", "EXTEND_X")
                   .save()
                   .groupBy("PXBarrel|PXForward", "SUM")
                   .save()

    #StandardSpecificationTrend_Num,
    #StandardSpecification2DProfile_Num,
    #*StandardSpecifications1D_Num
  )
)

SiPixelPhase1DigisNdigisPerFED = DefaultHisto.clone(
  name = "feddigis", # This is the same as above up to the ranges. maybe we 
  title = "Digis",   # should allow setting the range per spec, but OTOH a 
  xlabel = "digis",  # HistogramManager is almost free.
  range_min = 0,
  range_max = 1000,
  range_nbins = 200,
  dimensions = 0, 
  specs = cms.VPSet(
    # the double "FED" here is due to a "bug", caused by how the specs are
    # translated for step1. Interpret as "count by FED, extend by FED".
    Specification().groupBy("FED/FED/Event")
                   .reduce("COUNT")
                   .groupBy("FED")
                   .groupBy("", "EXTEND_Y")
                   .save(),
    Specification().groupBy("PXBarrel|PXForward/PXLayer|PXDisk/Lumisection/FED/FED/Event")
                   .reduce("COUNT")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/Lumisection/FED")
                   .reduce("MEAN")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/Lumisection", "EXTEND_Y")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk", "EXTEND_X")
                   .save()
                   .custom("ratio_to_average")
                   .save()
  )
)

SiPixelPhase1DigisEvents = DefaultHisto.clone(
  name = "eventrate",
  title = "Rate of Pixel Events",
  xlabel = "Lumisection",
  ylabel = "#Events",
  dimensions = 0,
  specs = cms.VPSet(
    Specification().groupBy("Lumisection")
                   .groupBy("", "EXTEND_X").save(),
    Specification().groupBy("BX")
                   .groupBy("", "EXTEND_X").save()
  )
)

SiPixelPhase1DigisHitmap = DefaultHisto.clone(
  name = "hitmap",
  title = "Position of digis on module",
  ylabel = "#digis",
  dimensions = 0,
  specs = cms.VPSet(
    Specification(PerModule).groupBy("PXBarrel|PXForward/PXLayer|PXDisk/DetId/row/col")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/DetId/row", "EXTEND_Y")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/DetId", "EXTEND_X")
                   .save(),
    Specification(PerModule).groupBy("PXBarrel|PXForward/PXLayer|PXDisk/DetId/col")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/DetId", "EXTEND_X")
                   .save(),
    Specification(PerModule).groupBy("PXBarrel|PXForward/PXLayer|PXDisk/DetId/row")
                   .groupBy("PXBarrel|PXForward/PXLayer|PXDisk/DetId", "EXTEND_X")
                   .save()

  )
)

SiPixelPhase1DigisDebug = DefaultHisto.clone(
  enabled = False,
  name = "debug",
  xlabel = "ladder #",
  range_min = 1,
  range_max = 64,
  range_nbins = 64,
  specs = cms.VPSet(
    Specification().groupBy("PXBarrel|PXForward/Shell|HalfCylinder/PXLayer|PXDisk/PXRing|/PXLadder|PXBlade") 
                   .save()
                   .reduce("MEAN")
                   .groupBy("PXBarrel|PXForward/Shell|HalfCylinder/PXLayer|PXDisk/PXRing|", "EXTEND_X")
                   .saveAll(),
  )
)

# This has to match the order of the names in the C++ enum.
SiPixelPhase1DigisConf = cms.VPSet(
  SiPixelPhase1DigisADC,
  SiPixelPhase1DigisNdigis,
  SiPixelPhase1DigisNdigisPerFED,
  SiPixelPhase1DigisEvents,
  SiPixelPhase1DigisHitmap,
  SiPixelPhase1DigisDebug
)

SiPixelPhase1DigisAnalyzer = cms.EDAnalyzer("SiPixelPhase1Digis",
        src = cms.InputTag("simSiPixelDigis"), 
        histograms = SiPixelPhase1DigisConf,
        geometry = SiPixelPhase1Geometry
)

SiPixelPhase1DigisHarvester = cms.EDAnalyzer("SiPixelPhase1DigisHarvester",
        histograms = SiPixelPhase1DigisConf,
        geometry = SiPixelPhase1Geometry
)
