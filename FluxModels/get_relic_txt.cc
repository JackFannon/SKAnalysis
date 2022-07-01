void get_relic_txt()
{
  const int nmodels = 16; 
  string inputname[ nmodels ]; 
    inputname[ 0 ]  = "nakazato_ih_max.txt";  // Nakazato 1
    inputname[ 1 ]  = "nakazato_nh_min.txt";  // Nakazato 2
    inputname[ 2 ]  = "horiuchi_xi2.5crit0.1.txt";  // Horiuchi 1
    inputname[ 3 ]  = "horiuchi_xi2.5crit0.5.txt";  // Horiuchi 2
    inputname[ 4 ]  = "horiuchi_flux.vec";  // Horiuchi 3
    inputname[ 5 ]  = "lma_flux.ext.vec";  // Ando 
    inputname[ 6 ]  = "faild_flux.vec";  // Lunardini 
    inputname[ 7 ]  = "malaney_flux.vec";  // Malaney 
    inputname[ 8 ]  = "woosley_flux.vec";  // Hartmann
    inputname[ 9 ]  = "ksw_flux.vec";  // Kaplinghat
    inputname[ 10 ] = "const_flux.vec";  // Totani 
    inputname[ 11 ] = "flux_cross_galais_ih.dat";  // Galais IH
    inputname[ 12 ] = "flux_cross_galais_nh.dat";  // Galais NH
    inputname[ 13 ] = "flux_cross_kresse_high_nh.dat";  // Kresse NH optimistic
    inputname[ 14 ] = "tabrizi_nh.txt";  // Tabrizi NH
    inputname[ 15 ] = "Plot_DSNB_nuebarNH_LimongiA01.dat";  // Limongi NH
  string outputname[ nmodels ]; 
    outputname[ 0 ]  = "nakazato1.txt";  // Nakazato 1
    outputname[ 1 ]  = "nakazato2.txt";  // Nakazato 2
    outputname[ 2 ]  = "horiuchi1.txt";  // Horiuchi 1
    outputname[ 3 ]  = "horiuchi2.txt";  // Horiuchi 2
    outputname[ 4 ]  = "horiuchi3.txt";  // Horiuchi 3
    outputname[ 5 ]  = "ando.txt";  // Ando 
    outputname[ 6 ]  = "lunardini.txt";  // Lunardini 
    outputname[ 7 ]  = "malaney.txt";  // Malaney 
    outputname[ 8 ]  = "hartmann.txt";  // Hartmann
    outputname[ 9 ]  = "kaplinghat.txt";  // Kaplinghat
    outputname[ 10 ] = "totani.txt";  // Totani 
    outputname[ 11 ] = "galaisIH.txt";  // Galais IH
    outputname[ 12 ] = "galaisNH.txt";  // Galais NH
    outputname[ 13 ] = "kresseNHhigh.txt";  // Kresse NH high
    outputname[ 14 ] = "tabriziNH.txt";  // Tabrizi NH 
    outputname[ 15 ] = "limongiNH.txt";  // Limongi NH
   

  for ( int imodel = 0; imodel < nmodels; imodel++ )
  {
    cout << "model" << " " << imodel << " ..." << endl; 
    cout << inputname[imodel] << endl;
    ifstream input( Form( "../flux_comparison/%s", inputname[ imodel ].c_str() ) );
    ofstream output( Form( "%s", outputname[ imodel ].c_str() ) );

    double enu[ 10000 ] = { 0. };
    double dflux[ 10000 ] = { 0. };
    int nlines = 0;
    while ( ! input.eof() ) {
      input >> enu[ nlines ] >> dflux[ nlines ];
      nlines++;
    }

    double elow = 3.; double ehigh = 35.; 
    int ntmp = ( (int)ehigh - (int)elow ) + 1; const int nbins = ntmp; 
    double eedge[ nbins ], nubflux[ nbins ];
    for ( int ibin = 0; ibin < nbins; ibin++ ) {
      eedge[ ibin ] = elow + (double)ibin; 
      //cout << eedge[ ibin ] << endl; 
    }

    int itmp = 0; 
    for ( int iline = 0; iline < nlines; iline++ ) {
      if ( itmp >= nbins ) break; 
      if ( enu[ iline ] < eedge[ itmp ] ) {
      }
      else if ( enu[ iline ] == eedge[ itmp ] ) {
	nubflux[ itmp ] = dflux[ iline ];
	itmp++;
      }
      else if ( enu[ iline ] > eedge[ itmp ] ) {
	if ( enu[ iline ] != enu[ iline - 1 ] ) {
	  nubflux[ itmp ] = dflux[ iline - 1 ] 
			  + ( dflux[ iline ] - dflux[ iline - 1 ] ) 
			  * ( eedge[ itmp ] - enu[ iline - 1 ] )/( enu[ iline ] - enu[ iline - 1 ] );
	}
	itmp++; 
      }
    }

    for ( int ibin = 0; ibin < nbins; ibin++ ) {
      //cout << eedge[ ibin ] << " " << nubflux[ ibin ] << endl; 
      output << eedge[ ibin ] << " " << nubflux[ ibin ] << "\n"; 
    }
  
    output.close(); 
  }

}


