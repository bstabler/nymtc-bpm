// work_coeff.h

// definitions for work journey mode choice coefficients
// estimated:
#define  IN_VEH_TIME -0.024760
#define  COST		 -0.000940
#define  FW_LE7_TR	 -0.189569
#define  FW_GT7_TR	 -0.121191
#define  FW_LE7_CR	 -0.064390
#define  FW_GT7_CR	 -0.035058
#define  XFER_WAIT	 -0.063705
#define  WALK		 -0.083115
#define  CR_DIST	  0.082178
#define  WAIT_TX	 -0.960420
#define  DA_IVT		 -0.0495
#define  INFORMLOT_TR -1.141941
#define  INFORMLOT_CR -0.981939
#define  HIAFW_2P	 -2.110745
#define  HIASW_2P	 -3.530455
#define  HIAMW_2P	 -5.101668
#define  HINOA_2P	  1.003327
#define  MDAFW_2P	 -1.090844
#define  MDASW_2P	 -3.297789
#define  MDAMW_2P	 -4.326583
#define  MDNOA_2P	  0.772031
#define  LONOA_2P	  1.470487
#define  UA_2P		  2.326664
#define  HIAFW_3P	 -4.276679
#define  HIASW_3P	 -5.169171
#define  HIAMW_3P	 -5.318120
#define  HINOA_3P	 -1.738179
#define  MDAFW_3P	 -3.096579
#define  MDASW_3P	 -4.783437
#define  MDAMW_3P	 -6.176054
#define  MDNOA_3P	  0.044855
#define  LONOA_3P	 -0.858608
#define  UA_3P		  2.073169
#define  HIAFW_4P	 -4.775963
#define  HIASW_4P	 -5.378198
#define  HIAMW_4P	 -7.888027
#define  MDAFW_4P	 -4.219091
#define  MDASW_4P	 -5.874584
#define  MDAMW_4P	 -7.608058
#define  LONOA_4P	 -1.588223
#define  UA_4P		  1.756741
#define  HIAFW_WT	 -1.024267
#define  HIASW_WT	 -3.479856
#define  HIAMW_WT	 -4.699071
#define  HINOA_WT	  3.984599
#define  MDAFW_WT	  0.415089
#define  MDASW_WT	 -2.624849
#define  MDAMW_WT	 -3.853138
#define  MDNOA_WT	  5.077691
#define  LONOA_WT	  3.754364
#define  UA_WT		  4.700619
#define  HIAFW_DT	 -1.390976
#define  HIASW_DT	 -4.757704
#define  HIAMW_DT	 -4.604698
#define  HINOA_DT	  1.090569
#define  MDAFW_DT	 -2.242928
#define  MDASW_DT	 -4.304562
#define  MDAMW_DT	 -4.785513
#define  MDNOA_DT	  2.308230
#define  UA_DT		  4.972256
#define  HIAFW_WC	 -2.766608
#define  HIASW_WC	 -6.031361
#define  HIAMW_WC	 -7.074308
#define  MDAFW_WC	 -1.028525
#define  MDASW_WC	 -4.929020
#define  MDAMW_WC	 -7.152079
#define  UA_WC		  4.921263
#define  HIAFW_DC	 -3.368443
#define  HIASW_DC	 -5.245563
#define  HIAMW_DC	 -6.056416
#define  MDAFW_DC	 -2.572197
#define  MDASW_DC	 -4.864669
#define  MDAMW_DC	 -6.384432
#define  UA_DC		  4.744090
#define  HIAFW_TX	 -2.987309
#define  HIASW_TX	 -6.771011
#define  HIAMW_TX	 -7.606931
#define  HINOA_TX	 -0.755943
#define  MDAFW_TX	 -3.966049
#define  MDASW_TX	 -11.11485
#define  MDAMW_TX	 -9.644041
#define  MDNOA_TX	 -1.009055
#define  LONOA_TX	 -7.462623
#define  UA_TX		  5.050109
#define  THETA151	  0.910038
#define  THETA152	  0.502606
#define  THETA151_X_THETA152 THETA151*THETA152

// not included in models:
#define  PARKING_COST 0.0
#define  UA_DA		  0.0

// restored constants
#define  MDNOA_4P	 -1.0777
#define  MDNOA_WC	  3.6341
#define  MDNOA_DC	  1.9790
#define  HINOA_4P	 -2.2375
#define  HINOA_WC	  2.2423
#define  HINOA_DC	 -0.8869

#define  LONOA_DT	  0.9849
#define  LONOA_WC	  2.3107
#define  LONOA_DC	  0.6556

#define  LOAFW_2P	 -0.3924
#define  LOASW_2P	 -2.5993
#define  LOAMW_2P	 -3.6281
#define  LOAFW_3P	 -4.0000
#define  LOASW_3P	 -5.6869
#define  LOAMW_3P	 -7.0795
#define  LOAFW_4P	 -4.7297
#define  LOASW_4P	 -6.3851
#define  LOAMW_4P	 -8.1186
#define  LOAFW_WT	 -0.9082
#define  LOASW_WT	 -3.9482
#define  LOAMW_WT	 -5.1765
#define  LOAFW_DT	 -3.5663
#define  LOASW_DT	 -5.6279
#define  LOAMW_DT	 -6.1088
#define  LOAFW_WC	 -2.3519
#define  LOASW_WC	 -6.2523
#define  LOAMW_WC	 -8.4754
#define  LOAFW_DC	 -3.8955
#define  LOASW_DC	 -6.1880
#define  LOAMW_DC	 -7.7078
#define  LOAFW_TX	 -10.4196
#define  LOASW_TX	 -17.5684
#define  LOAMW_TX	 -16.0976






