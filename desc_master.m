%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-
%- Set Parameters
%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-
%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-
str_class='boat';
cell_str_methods={'rank','hard','beta2'};	cell_str_methods={'hard'};
ia_patch_sizes=[51 41 61];					ia_patch_sizes=[51];
ia_patch_postsmooth=[1 0 3];				ia_patch_postsmooth=[1];
ba_split_regions=[false true];				ba_split_regions=false;
fa_ell_fit_threshold=[10 30];				fa_ell_fit_threshold=[10];
fa_patch_resolution=[0.5 1.0];				fa_patch_resolution=[1.0];
%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-

init;

%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-
%- Create Iterator
%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-
iter=IteratorsNeo2;
map_iterators=HashNeo({...
	{'method',cell_str_methods},...
	{'patch_size',ia_patch_sizes},...
	{'patch_postsmooth',ia_patch_postsmooth},...
	{'split_region',ba_split_regions},...
	{'ell_fit_threshold',fa_ell_fit_threshold},...
	{'patch_resolution',fa_patch_resolution},...
	});
for key_iterator=map_iterators.keys,
	key=key_iterator{1};
	iter.AddIterator(key,map_iterators(key));
end
%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-%-

while iter.Iterate,
	%- Extract the variables
	hash_values=iter.GetValues;
	
	%- Do not split regions if the binning method is 'beta2'
	if strcmp(hash_values('method'),'beta2') && hash_values('split_region'),
		continue;
	end

	disp(hash_values.keys);
% 	desc9_eval_1_helper(str_class,...
% 		hash_values('method'),...
% 		hash_values('patch_size'),...
% 		hash_values('patch_postsmooth'),...
% 		hash_values('split_region'),...
% 		hash_values('ell_fit_threshold'),...
% 		hash_values('patch_resolution'));
end