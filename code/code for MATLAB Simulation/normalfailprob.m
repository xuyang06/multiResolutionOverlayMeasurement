function [ failprob ] = normalfailprob()
%UNTITLED22 Summary of this function goes here
%   Detailed explanation goes here

failprob = 0.0;
norm_prob = normrnd(0.05,0.01);
if norm_prob <= 0
    failprob = 0;
elseif norm_prob >= 1
    failprob = 1;
else
    failprob = norm_prob;

end

