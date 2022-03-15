clear
[y1,fs]=audioread('lz.wav');
y1=y1(:,1);
N1=length(y1);
n=0:N1-1;
t=n/fs;
f=0:fs/N1:fs*(N1-1)/N1;
Y1=fft(y1,N1);

figure(1)
subplot(2,2,1);
plot(t,y1);
xlabel('ʱ��t');
ylabel('����');
title('ԭ�ź�ʱ����ͼ');
subplot(2,2,2);
plot(f,abs(Y1));
xlabel('Ƶ��f');
ylabel('����');
title('ԭ�ź�Ƶ��ͼ');
y2=voice(y1,1);                               %1Ϊf��ʼֵ                                   
N2=length(y2);
k=0:1:N2-1;
t=k/fs;
f=k*fs/N2;
Y2=fft(y2,N2);

subplot(2,2,3);
plot(t,y2);
xlabel('ʱ��t');
ylabel('����');
title('�����ź�ʱ����ͼ');
subplot(2,2,4);
plot(f,abs(Y2));
xlabel('Ƶ��f');
ylabel('����');
title('�����ź�Ƶ��ͼ');

sound(y2,fs);                         %���ű��������Ƶ������ر����������д������롮clear sound��
filename = '������-������-������.wav';
audiowrite(filename,y2,fs);            %�����������Ƶ��wav��ʽ���
 


function Y=voice(y1,f)  
f=round(f*910);                            %�ı�n����ʵ�������仯����ʼֵ1000.
d=resample(y1,f,1000);    
W=400;
Wov=W/2;
Kmax=W*2;
Wsim=Wov;
xdecim=8;
kdecim=2;
X=d';
F=f/1000;                      %�ı�Fʱ���������ӿɸı����٣�������ټӿ죬��֮����
Ss=W-Wov;
xpts=size(X,2);
ypts=round(xpts/F);
Y=zeros(1,ypts);
xfwin=(1:Wov)/(Wov+1);
ovix=(1-Wov):0;
newix=1:(W-Wov);
simix=(1:xdecim:Wsim)-Wsim;
padX=[zeros(1,Wsim),X,zeros(1,Kmax+W-Wov)];
Y(1:Wsim)=X(1:Wsim);
lastxpos=0;
km=0;
for ypos=Wsim:Ss:(ypts-W)
xpos=round(F*ypos);
kmpred=km+(xpos-lastxpos);
lastxpos=xpos;
if(kmpred<=Kmax)
   km=kmpred;
else
ysim=Y(ypos+simix);
rxy=zeros(1,Kmax+1);
rxx=zeros(1,Kmax+1);
Kmin=0;
for k=Kmin:kdecim:Kmax
xsim=padX(Wsim+xpos+k+simix);
rxx(k+1)=norm(xsim);
rxy(k+1)=(ysim*xsim');
end
Rxy=(rxx~=0).*rxy./(rxx+(rxx==0));
km=min(find(Rxy==max(Rxy))-1);
end
xabs=xpos+km;
Y(ypos+ovix)=((1-xfwin).*Y(ypos+ovix))+(xfwin.*padX(Wsim+xabs+ovix));
Y(ypos+newix)=padX(Wsim+xabs+newix);
end
end