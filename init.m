function [pmtrs,folders]=init(pmtrs)

if nargin<1,
	pmtrs.class='boat';
	pmtrs.intpt.type='hesaff';
	pmtrs.intpt.thres_har=3200;
	pmtrs.intpt.thres_hes=1600;
	
	pmtrs.desc.nbrhd_shape='ellipse';
	pmtrs.desc.method='rank'; % can be rank, hard,
	pmtrs.desc.nbrhd_scaling=1.0;
	pmtrs.desc.nbrhd_smoothing_method=0.5;
	pmtrs.desc.insplits=2;
	pmtrs.desc.version='v7.2';
	
	pmtrs.matching.scaling_ell=2.0;
	pmtrs.matching.method='normalize-relative';
	pmtrs.matching.nbrhd_rotate=1;
	pmtrs.matching.nbrhd_scale=1;
	pmtrs.matching.resolution=25;
end

strclass=pmtrs.class;
strintpttype=pmtrs.intpt.type;

% add folders and initialize folders
str_fo_home=getenv('HOME');
if ~exist('ProgressBar','file'),
	addpath([str_fo_home '/neo-human/programs/utils/matlab']);
end

folders.root=[str_fo_home '/neo-human/programs/code-name/yak/'];
folders.oxford=[str_fo_home '/neo-human/data/oxford/affine-covariant-regions'];
folders.data=[folders.root '/oxford'];

for strpathrelative={'corres','cslbp','ellipse','extract-features','get','nbrhd','repeatability','roc','show','utils'}
	CreateAndAddPath([folders.root '/' strpathrelative{1}]);
end

% assign the correct image extension
folders.ext=GetImageExtension(pmtrs.class);

folders.extractfeat=[folders.root '/extract-features/extract_features_64bit.ln'];
folders.computedesc=[folders.root '/extract-features/compute_descriptors.ln'];

for strpathrelative={'desc','feat','roc'}
	mkdirNeo([folders.data '/' strclass '/' strintpttype '/' strpathrelative{1}]);
end

%-%- --------------------------------------------------
%-%- compile
map_mexfiles=HashNeo({...
	{'iir_gauss','nbrhd'},...
	{'ellipse_overlap5','ellipse'},...
	{'ellipse_overlap10','ellipse'},...
	{'c_eoverlap','roc'},...
	{'descdist','repeatability'},...
	});
for key_file=map_mexfiles.keys
	key=key_file{1};
	if ~exist(key,'file'),
		CompileFile(key,[folders.root '/' map_mexfiles(key)]);
	end
end

end

function CompileFile(str_fi_wo_extension,str_fo)
strOldDir=pwd;
cd(str_fo);
str_fi_extension='cxx';
if ~exist([str_fi_wo_extension '.' str_fi_extension],'file')
	str_fi_extension='cpp';
end
mex([str_fi_wo_extension '.' str_fi_extension]);
cd(strOldDir);
end

function CreateAndAddPath(args)
if ~iscell(args), args={args}; end
for strpath=args
	str_path=strpath{1};
	if ~exist(str_path,'file')
		mkdirNeo(str_path);
		addpath(str_path);
	end
end
end



%-%- --------------------------------------------------
