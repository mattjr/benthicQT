#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <iostream>
#include <fstream>
#include <memory>
#include <QProgressDialog>
template<typename Elem, typename Tr = std::char_traits<Elem> >
class progress_streambuf: public std::basic_filebuf<Elem, Tr>
{
public:
        typedef Tr traits_type;

        typedef std::basic_filebuf<Elem, Tr> base_type;
        typedef typename traits_type::int_type int_type;
        explicit progress_streambuf(const std::string &filename,QProgressDialog *pd)
        :	base_type(),
                count_(0),
                pd_(pd),
                div_(100)
        {
                if (this->open(filename.c_str(), std::ios_base::in | std::ios_base::binary))
                {
                        size_ = static_cast<int>(this->pubseekoff(0, std::ios_base::end, std::ios_base::in));
                        pd_->setRange(0,div_);
                        pd_->setValue(0);
                        this->pubseekoff(0, std::ios_base::beg, std::ios_base::in);
                }
        }

protected:
        virtual int_type uflow()
        {
                int_type v = base_type::uflow();
                count_ += this->egptr() - this->gptr();
                int prog=int(((size_ - this->showmanyc())/(double)size_)*div_) ;
                pd_->setValue( prog );
              //  qDebug() << prog << " "<<size_;
                qApp->processEvents();
                return v;
        }

private:
        int count_;
        int div_;
        int size_;
        QProgressDialog *pd_;
};

typedef progress_streambuf<char> progbuf;

#endif // PROGRESSBAR_H
