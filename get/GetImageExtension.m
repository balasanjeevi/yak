
function strExtension=GetImageExtension(str_class)

if strcmp(str_class,'boat'),	strExtension='pgm';
else,						strExtension='ppm';
end
