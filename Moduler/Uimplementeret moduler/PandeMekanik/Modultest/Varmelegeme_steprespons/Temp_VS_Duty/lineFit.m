function [a,b] = lineFit(x,y)
if(length(x) ~= length(y))
    error('x and y must have same length')
end
if ~(isvector(x) || isvector(y))
    error('x and y must be vectors')
end

N = length(x);



X = [ones(N,1) x(:)];
Y = (X.'*X)\(X.'*y(:));
a = (Y(2));
b = (Y(1));