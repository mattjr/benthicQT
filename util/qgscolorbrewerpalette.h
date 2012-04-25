#ifndef QGSCOLORBREWERPALETTE_H
#define QGSCOLORBREWERPALETTE_H
extern const char* brewerStringSeq;
extern const char *brewerStringDiv;
extern const char* brewerStringQual;

class QgsColorBrewerPalette
{
  public:
    QgsColorBrewerPalette(){
            loadDivColors();
            loadQualColors();
            loadSeqColors();
    }
    QStringList listSchemesQual(int numClasses){
        QStringList lst;
        std::map< QString , map<int,QList<QColor> > >::iterator it=qualColors.begin();
        for(;it !=qualColors.end(); it++){
            if(it->second.count(numClasses)){
                lst.push_back((it->first));
            }
        }
        return lst;
    }
     void loadSeqColors(  )
    {
      QString palette( brewerStringSeq );
      QStringList list = palette.split( QChar( '\n' ) );
      foreach( QString entry, list )
      {
          QList<QColor> pal;

        QStringList items = entry.split( QChar( '-' ) );
        if ( items.count() != 3  )
          continue;
        QStringList colors = items[2].split( QChar( ' ' ) );
        foreach( QString clr, colors )
        pal << parseColor( clr );
         seqColors[items[0]]=pal;
      }
    }
     void loadDivColors(  )
    {
      QString palette( brewerStringDiv );
      QStringList list = palette.split( QChar( '\n' ) );
      foreach( QString entry, list )
      {
          QList<QColor> pal;

        QStringList items = entry.split( QChar( '-' ) );
        if ( items.count() != 3  )
          continue;
        QStringList colors = items[2].split( QChar( ' ' ) );
        foreach( QString clr, colors )
        pal << parseColor( clr );
         divColors[items[0]]=pal;
      }
    }

     void loadQualColors(  )
    {
      QString palette( brewerStringQual );
      QStringList list = palette.split( QChar( '\n' ) );
      foreach( QString entry, list )
      {
          QList<QColor> pal;

        QStringList items = entry.split( QChar( '-' ) );
        if ( items.count() != 3  )
          continue;
        QStringList colors = items[2].split( QChar( ' ' ) );
        foreach( QString clr, colors )
        pal << parseColor( clr );
        std::pair<int,QList<QColor> >  p =std::make_pair<int,QList<QColor> > ( items[1].toInt(),pal);
        qualColors[items[0]].insert(p);
      }
    }

    QStringList listSchemesSeq(){
        QStringList lst;
       std::map< QString, QList<QColor> >::iterator it=seqColors.begin();
        for(;it !=seqColors.end(); it++){
                lst.push_back((it->first));
        }
        return lst;
    }

    QStringList listSchemesDiv(){
        QStringList lst;
       std::map< QString, QList<QColor> >::iterator it=divColors.begin();
        for(;it !=divColors.end(); it++){
                lst.push_back((it->first));
        }
        return lst;
    }
    QList<QColor> getQualScheme(QString name,int numClasses){
        if(qualColors.count(name)){
            if(qualColors[name].count(numClasses))
                return qualColors[name][numClasses];
        }


        QList<QColor> null_lst;
        null_lst.push_back(QColor(255,0,0));
        return null_lst;

    }
    QList<QColor> getDivScheme(QString name){
        if(divColors.count(name)){

                return divColors[name];
        }


        QList<QColor> null_lst;
        return null_lst;

    }


    static int max_pal_size(){
        return 12;
    }

   /* static QList<QColor> listSchemeColors( QString schemeName, int colors )
    {
      QList<QColor> pal;
      QString palette( brewerString );
      QStringList list = palette.split( QChar( '\n' ) );
      foreach( QString entry, list )
      {
        QStringList items = entry.split( QChar( '-' ) );
        if ( items.count() != 3 || items[0] != schemeName || items[1].toInt() != colors )
          continue;
        QStringList colors = items[2].split( QChar( ' ' ) );
        foreach( QString clr, colors )
        pal << parseColor( clr );
      }
      return pal;
    }

    static std::map< std::string , QList<QColor> > getSchemeColorsDiv( QString schemeName, int colors )
    {
      QList<QColor> pal;
      QString palette( brewerStringDiv );
      QStringList list = palette.split( QChar( '\n' ) );
      foreach( QString entry, list )
      {
        QStringList items = entry.split( QChar( '-' ) );
        if ( items.count() != 3 || items[0] != schemeName || items[1].toInt() != colors )
          continue;
        QStringList colors = items[2].split( QChar( ' ' ) );
        foreach( QString clr, colors )
        pal << parseColor( clr );
      }
      return pal;
    }

    static QStringList listSchemes()
    {
      QStringList schemes;

      QString palette( brewerString );
      QStringList list = palette.split( QChar( '\n' ) );
      foreach( QString entry, list )
      {
        QStringList items = entry.split( QChar( '-' ) );
        if ( items.count() != 3 ) continue;
        if ( !schemes.contains( items[0] ) )
          schemes << items[0];
      }
      return schemes;
    }

    static QStringList listSchemes(int numClasses)
    {
      QStringList schemes;

      QString palette( brewerString );
      QStringList list = palette.split( QChar( '\n' ) );
      foreach( QString entry, list )
      {
        QStringList items = entry.split( QChar( '-' ) );
        if ( items.count() != 3  || items[1].toInt() != numClasses) continue;
        if ( !schemes.contains( items[0] ) )
          schemes << items[0];
      }
      return schemes;
    }

    static QList<int> listSchemeVariants( QString schemeName )
    {
      QList<int> variants;

      QString palette( brewerString );
      QStringList list = palette.split( QChar( '\n' ) );
      foreach( QString entry, list )
      {
        QStringList items = entry.split( QChar( '-' ) );
        if ( items.count() != 3 || items[0] != schemeName ) continue;
        variants << items[1].toInt();
      }

      return variants;
    }

*/
    static QColor parseColor( QString color )
    {
      QStringList p = color.split( QChar( ',' ) );
      if ( p.count() != 3 ) return QColor();
      return QColor( p[0].toInt(), p[1].toInt(), p[2].toInt() );
    }
public:
    std::map< QString , QList<QColor> > divColors;
    std::map< QString, QList<QColor> > seqColors;

    std::map< QString , map<int,QList<QColor> > > qualColors;

};

/*
Apache-Style Software License for ColorBrewer software and ColorBrewer Color Schemes

Copyright (c) 2002 Cynthia Brewer, Mark Harrower, and The Pennsylvania State University.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*/

// extracted ColorBrewer data
const char* brewerStringSeq =
        "OrRd-9-255,247,236 254,232,200 253,212,158 253,187,132 252,141,89 239,101,72 215,48,31 179,0,0 127,0,0\n"
        "PuBu-9-255,247,251 236,231,242 208,209,230 166,189,219 116,169,207 54,144,192 5,112,176 4,90,141 2,56,88\n"
        "BuPu-9-247,252,253 224,236,244 191,211,230 158,188,218 140,150,198 140,107,177 136,65,157 129,15,124 77,0,75\n"
        "Oranges-9-255,245,235 254,230,206 253,208,162 253,174,107 253,141,60 241,105,19 217,72,1 166,54,3 127,39,4\n"
        "BuGn-9-247,252,253 229,245,249 204,236,230 153,216,201 102,194,164 65,174,118 35,139,69 0,109,44 0,68,27\n"
        "YlOrBr-9-255,255,229 255,247,188 254,227,145 254,196,79 254,153,41 236,112,20 204,76,2 153,52,4 102,37,6\n"
        "YlGn-9-255,255,229 247,252,185 217,240,163 173,221,142 120,198,121 65,171,93 35,132,67 0,104,55 0,69,41\n"
        "Reds-9-255,245,240 254,224,210 252,187,161 252,146,114 251,106,74 239,59,44 203,24,29 165,15,21 103,0,13\n"
        "RdPu-9-255,247,243 253,224,221 252,197,192 250,159,181 247,104,161 221,52,151 174,1,126 122,1,119 73,0,106\n"
        "Greens-9-247,252,245 229,245,224 199,233,192 161,217,155 116,196,118 65,171,93 35,139,69 0,109,44 0,68,27\n"
        "YlGnBu-9-255,255,217 237,248,177 199,233,180 127,205,187 65,182,196 29,145,192 34,94,168 37,52,148 8,29,88\n"
        "BrBG-3-216,179,101 245,245,245 90,180,172\n"
        "Purples-9-252,251,253 239,237,245 218,218,235 188,189,220 158,154,200 128,125,186 106,81,163 84,39,143 63,0,125\n"
        "GnBu-9-247,252,240 224,243,219 204,235,197 168,221,181 123,204,196 78,179,211 43,140,190 8,104,172 8,64,129\n"
        "Greys-9-255,255,255 240,240,240 217,217,217 189,189,189 150,150,150 115,115,115 82,82,82 37,37,37 0,0,0\n"
        "YlOrRd-3-255,237,160 254,178,76 240,59,32\n"
        "YlOrRd-9-255,255,204 255,237,160 254,217,118 254,178,76 253,141,60 252,78,42 227,26,28 189,0,38 128,0,38\n"
        "PuRd-9-247,244,249 231,225,239 212,185,218 201,148,199 223,101,176 231,41,138 206,18,86 152,0,67 103,0,31\n"
        "Blues-9-247,251,255 222,235,247 198,219,239 158,202,225 107,174,214 66,146,198 33,113,181 8,81,156 8,48,107\n"
        "PuBuGn-9-255,247,251 236,226,240 208,209,230 166,189,219 103,169,207 54,144,192 2,129,138 1,108,89 1,70,54\n";
const char *brewerStringQual=
        "Accent-3-127,201,127 190,174,212 253,192,134\n"
        "Accent-4-127,201,127 190,174,212 253,192,134 255,255,153\n"
        "Accent-5-127,201,127 190,174,212 253,192,134 255,255,153 56,108,176\n"
        "Accent-6-127,201,127 190,174,212 253,192,134 255,255,153 56,108,176 240,2,127\n"
        "Accent-7-127,201,127 190,174,212 253,192,134 255,255,153 56,108,176 240,2,127 191,91,23\n"
        "Accent-8-127,201,127 190,174,212 253,192,134 255,255,153 56,108,176 240,2,127 191,91,23 102,102,102\n"
        "Dark2-3-27,158,119 217,95,2 117,112,179\n"
        "Dark2-4-27,158,119 217,95,2 117,112,179 231,41,138\n"
        "Dark2-5-27,158,119 217,95,2 117,112,179 231,41,138 102,166,30\n"
        "Dark2-6-27,158,119 217,95,2 117,112,179 231,41,138 102,166,30 230,171,2\n"
        "Dark2-7-27,158,119 217,95,2 117,112,179 231,41,138 102,166,30 230,171,2 166,118,29\n"
        "Dark2-8-27,158,119 217,95,2 117,112,179 231,41,138 102,166,30 230,171,2 166,118,29 102,102,102\n"
        "Paired-3-166,206,227 31,120,180 178,223,138\n"
        "Paired-4-166,206,227 31,120,180 178,223,138 51,160,44\n"
        "Paired-5-166,206,227 31,120,180 178,223,138 51,160,44 251,154,153\n"
        "Paired-6-166,206,227 31,120,180 178,223,138 51,160,44 251,154,153 227,26,28\n"
        "Paired-7-166,206,227 31,120,180 178,223,138 51,160,44 251,154,153 227,26,28 253,191,111\n"
        "Paired-8-166,206,227 31,120,180 178,223,138 51,160,44 251,154,153 227,26,28 253,191,111 255,127,0\n"
        "Paired-9-166,206,227 31,120,180 178,223,138 51,160,44 251,154,153 227,26,28 253,191,111 255,127,0 202,178,214\n"
        "Paired-10-166,206,227 31,120,180 178,223,138 51,160,44 251,154,153 227,26,28 253,191,111 255,127,0 202,178,214 106,61,154\n"
        "Paired-11-166,206,227 31,120,180 178,223,138 51,160,44 251,154,153 227,26,28 253,191,111 255,127,0 202,178,214 106,61,154 255,255,153\n"
        "Paired-12-166,206,227 31,120,180 178,223,138 51,160,44 251,154,153 227,26,28 253,191,111 255,127,0 202,178,214 106,61,154 255,255,153 177,89,40\n"
        "Pastel2-3-179,226,205 253,205,172 203,213,232\n"
        "Pastel2-4-179,226,205 253,205,172 203,213,232 244,202,228\n"
        "Pastel2-5-179,226,205 253,205,172 203,213,232 244,202,228 230,245,201\n"
        "Pastel2-6-179,226,205 253,205,172 203,213,232 244,202,228 230,245,201 255,242,174\n"
        "Pastel2-7-179,226,205 253,205,172 203,213,232 244,202,228 230,245,201 255,242,174 241,226,204\n"
        "Pastel2-8-179,226,205 253,205,172 203,213,232 244,202,228 230,245,201 255,242,174 241,226,204 204,204,204\n"
        "Pastel1-3-251,180,174 179,205,227 204,235,197\n"
        "Pastel1-4-251,180,174 179,205,227 204,235,197 222,203,228\n"
        "Pastel1-5-251,180,174 179,205,227 204,235,197 222,203,228 254,217,166\n"
        "Pastel1-6-251,180,174 179,205,227 204,235,197 222,203,228 254,217,166 255,255,204\n"
        "Pastel1-7-251,180,174 179,205,227 204,235,197 222,203,228 254,217,166 255,255,204 229,216,189\n"
        "Pastel1-8-251,180,174 179,205,227 204,235,197 222,203,228 254,217,166 255,255,204 229,216,189 253,218,236\n"
        "Pastel1-9-251,180,174 179,205,227 204,235,197 222,203,228 254,217,166 255,255,204 229,216,189 253,218,236 242,242,242\n"
        "Set2-3-102,194,165 252,141,98 141,160,203\n"
        "Set2-4-102,194,165 252,141,98 141,160,203 231,138,195\n"
        "Set2-5-102,194,165 252,141,98 141,160,203 231,138,195 166,216,84\n"
        "Set2-6-102,194,165 252,141,98 141,160,203 231,138,195 166,216,84 255,217,47\n"
        "Set2-7-102,194,165 252,141,98 141,160,203 231,138,195 166,216,84 255,217,47 229,196,148\n"
        "Set2-8-102,194,165 252,141,98 141,160,203 231,138,195 166,216,84 255,217,47 229,196,148 179,179,179\n"
        "Set1-3-228,26,28 55,126,184 77,175,74\n"
        "Set1-4-228,26,28 55,126,184 77,175,74 152,78,163\n"
        "Set1-5-228,26,28 55,126,184 77,175,74 152,78,163 255,127,0\n"
        "Set1-6-228,26,28 55,126,184 77,175,74 152,78,163 255,127,0 255,255,51\n"
        "Set1-7-228,26,28 55,126,184 77,175,74 152,78,163 255,127,0 255,255,51 166,86,40\n"
        "Set1-8-228,26,28 55,126,184 77,175,74 152,78,163 255,127,0 255,255,51 166,86,40 247,129,191\n"
        "Set1-9-228,26,28 55,126,184 77,175,74 152,78,163 255,127,0 255,255,51 166,86,40 247,129,191 153,153,153\n"
        "Set3-3-141,211,199 255,255,179 190,186,218\n"
        "Set3-4-141,211,199 255,255,179 190,186,218 251,128,114\n"
        "Set3-5-141,211,199 255,255,179 190,186,218 251,128,114 128,177,211\n"
        "Set3-6-141,211,199 255,255,179 190,186,218 251,128,114 128,177,211 253,180,98\n"
        "Set3-7-141,211,199 255,255,179 190,186,218 251,128,114 128,177,211 253,180,98 179,222,105\n"
        "Set3-8-141,211,199 255,255,179 190,186,218 251,128,114 128,177,211 253,180,98 179,222,105 252,205,229\n"
        "Set3-9-141,211,199 255,255,179 190,186,218 251,128,114 128,177,211 253,180,98 179,222,105 252,205,229 217,217,217\n"
        "Set3-10-141,211,199 255,255,179 190,186,218 251,128,114 128,177,211 253,180,98 179,222,105 252,205,229 217,217,217 188,128,189\n"
        "Set3-11-141,211,199 255,255,179 190,186,218 251,128,114 128,177,211 253,180,98 179,222,105 252,205,229 217,217,217 188,128,189 204,235,197\n"
        "Set3-12-141,211,199 255,255,179 190,186,218 251,128,114 128,177,211 253,180,98 179,222,105 252,205,229 217,217,217 188,128,189 204,235,197 255,237,111\n";

const char* brewerStringDiv =
        "Rainbow-7-255,0,0 255,255,0 0,255,0 0,255,255 0,0,255 255,0,255 255,0,0\n"
        "Jet-9-0,0,127 0,0,255 0,127,255 0,255,255 127,255,127 255,255,0 255,127,0 255,0,0 127,0,0\n"
        "BrBG-11-84,48,5 140,81,10 191,129,45 223,194,125 246,232,195 245,245,245 199,234,229 128,205,193 53,151,143 1,102,94 0,60,48\n"
        "PiYG-11-142,1,82 197,27,125 222,119,174 241,182,218 253,224,239 247,247,247 230,245,208 184,225,134 127,188,65 77,146,33 39,100,25\n"
        "PRGn-11-64,0,75 118,42,131 153,112,171 194,165,207 231,212,232 247,247,247 217,240,211 166,219,160 90,174,97 27,120,55 0,68,27\n"
        "PuOr-11-127,59,8 179,88,6 224,130,20 253,184,99 254,224,182 247,247,247 216,218,235 178,171,210 128,115,172 84,39,136 45,0,75\n"
        "RdBu-11-103,0,31 178,24,43 214,96,77 244,165,130 253,219,199 247,247,247 209,229,240 146,197,222 67,147,195 33,102,172 5,48,97\n"
        "RdGy-11-103,0,31 178,24,43 214,96,77 244,165,130 253,219,199 255,255,255 224,224,224 186,186,186 135,135,135 77,77,77 26,26,26\n"
        "RdYlGn-11-165,0,38 215,48,39 244,109,67 253,174,97 254,224,139 255,255,191 217,239,139 166,217,106 102,189,99 26,152,80 0,104,55\n"
        "RdYlBu-11-165,0,38 215,48,39 244,109,67 253,174,97 254,224,144 255,255,191 224,243,248 171,217,233 116,173,209 69,117,180 49,54,149\n"
        "Spectral-11-158,1,66 213,62,79 244,109,67 253,174,97 254,224,139 255,255,191 230,245,152 171,221,164 102,194,165 50,136,189 94,79,162\n";

#endif // QGSCOLORBREWERPALETTE_H
