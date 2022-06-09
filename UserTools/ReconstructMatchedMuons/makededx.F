        subroutine makededx(e,mdir,ihcab,qisk,tisk,xyzpm,
     ,              nqisk,run,watert,dedx)
        implicit none

#include "skwt.h"
#include "skday.h"
#include "skhead.h"
#include "skprint.h"

        real length,e(4),mdir(3),distance,skcoverage
        external mujecttime
        integer mujecttime,mujresult,nqisk
        real kpmt(4),kentry(4),kdist(2),kdir(3),cosang(2)
        real dedx(200)
        integer i,j,ihcab(11146),num,run,stop_bin,ti
        real qisk(11146),tisk(11146),watert,emit(3)
        real*4 xyzpm(3,11146)
        real phi,theta,costh,cospm,skcor

        do i = 1,200
           dedx(i) = 0
        end do

c        call skwt
c        call skrunday

        kentry(1) = e(1)
        kentry(2) = e(2)
        kentry(3) = e(3)
        kentry(4) = e(4)/29.94
        
        kdir(1) = mdir(1)
        kdir(2) = mdir(2)
        kdir(3) = mdir(3)

c        call lfwater(relapse(run),watert)

        do i=1,nqisk
           ti= ihcab(i)

           if(qisk(ti).ge.8) then
            
              kdist(1) = 0
              kdist(2) = 0
               
              kpmt(1) = xyzpm(1,ti)
              kpmt(2) = xyzpm(2,ti)
              kpmt(3) = xyzpm(3,ti)
              kpmt(4) = tisk(ti)

              mujresult = mujecttime(kentry,kdir,kpmt,kdist,cosang)
              do j = 1,mujresult
                 stop_bin=(kdist(j)/50.)+51.
                 emit(1) = kentry(1)+kdist(j)*kdir(1)
                 emit(2) = kentry(2)+kdist(j)*kdir(2)
                 emit(3) = kentry(3)+kdist(j)*kdir(3)
                 
             call lfhitgeom(emit,kdir,ti,distance,costh,cospm,theta,phi)

                 if(skcoverage(theta,phi).ne.0) then
                    skcor = 0.4041/skcoverage(theta,phi)
                 else
                    skcor = 0
                 end if

             if((stop_bin.ge.1).and.(stop_bin.le.200)) then
                dedx(stop_bin)=dedx(stop_bin)+qisk(ti)
     ,                     *exp(distance/watert)*skcor

             end if
 14          continue
          end do
       end if
      end do

      return
      end
