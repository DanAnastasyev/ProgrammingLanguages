-module(main).
-export([encode_string/2, decode_string/1]).
-export([main/3]).
-define(BPP, 3).
-define(EOS, 0).
-include("image_rec.hrl").

load(bmp, FileName) ->
  case file:read_file(FileName) of
    {ok, Contents} -> parse_contents(Contents);
    SomethingElse  -> SomethingElse
  end.

parse_contents(Contents) ->
  case Contents of
    <<"BM",_:64,Off:32/little,_:32,
        Width:32/signed-little,
        Height:32/signed-little,
        _Rest/binary>> ->
      Headers = binary_part(Contents,0,Off),
      PixelDataSize = size(Contents)-Off,
      PixelData = binary_part(Contents,Off,PixelDataSize),
      Image = #image{
        width = Width,
        height = Height,
        headers = Headers,
        contents = PixelData
      },
    {ok, Image};
    _ -> {error, wrong_format}
  end.

save_image(Image, Path) ->
  file:write_file(Path, Image#image.headers, [write]),
  file:write_file(Path, Image#image.contents, [append]).

get_size(Image) ->
  W = Image#image.width,
  H = Image#image.height,
  {W, H}.

row_size(Image) ->
  {W, _} = get_size(Image),
  MeaningfullBytes = W * ?BPP,
  MeaningfullBytes + (MeaningfullBytes rem 4).

pixel_position(Image, {X,Y}) ->
  RowSize = row_size(Image),
  RowSize * Y + X * ?BPP.

get_pixel(Image, Point) ->
  Address = pixel_position(Image, Point),
  Contents = Image#image.contents,
  binary_part(Contents, Address, ?BPP).

get_pixels(Image) ->
  {W, H} = get_size(Image),
  [get_pixel(Image, {XCoord, YCoord}) ||
    YCoord <- lists:seq(0, H - 1), XCoord <- lists:seq(0, W - 1)].

convert_pixel(Pixel, StringPart) ->
  <<BRest:6, _:2, GRest:6, _:2, RRest:6, _:2>> = Pixel,
  <<NewB:2, NewG:2, NewR:2>> = <<StringPart:6>>,
  <<BRest:6, NewB:2, GRest:6, NewG:2, RRest:6, NewR:2>>.

convert_to_pixel_bytes(<<>>) -> [?EOS];
convert_to_pixel_bytes(<<P:6, Rest/bitstring>>) ->
  [P| convert_to_pixel_bytes(Rest)].

lenient_zipwith(_, [], _) -> [];
lenient_zipwith(_, Xs, []) -> Xs;
lenient_zipwith(Fun, [X|Xs], [Y|Ys]) -> [Fun(X, Y) | lenient_zipwith(Fun, Xs, Ys)].

encode_string(Image, String) ->
  Bytes = unicode:characters_to_binary(String),
  PixelBytes = convert_to_pixel_bytes(Bytes),

  Pixels = get_pixels(Image),
  NewPixels = lenient_zipwith(fun(X, Y) -> convert_pixel(X, Y) end, Pixels, PixelBytes),
  NewContent = << <<Pixel/binary>> || Pixel <- NewPixels >>,

  #image{
    width = Image#image.width,
    height = Image#image.height,
    headers = Image#image.headers,
    contents = NewContent
  }.

decode_pixel(Pixel) ->
  <<_:6, B:2, _:6, G:2, _:6, R:2>> = Pixel,
  <<Res:6>> = <<B:2, G:2, R:2>>,
  Res.

decode_pixels([Pixel| Pixels], Acc) ->
  StringPart = decode_pixel(Pixel),
  if StringPart =:= ?EOS -> Acc;
     true                -> decode_pixels(Pixels, [Acc | <<StringPart:6>>])
  end.

decode_pixels(Pixels) -> decode_pixels(Pixels, []).

decode_string(Image) ->
  Pixels = get_pixels(Image),
  list_to_bitstring(decode_pixels(Pixels)).

main(FileName, EncodeString, OutputFileNam) ->
  {ok, Image} = load(bmp, FileName),
  NewImage = encode_string(Image, EncodeString),

  save_image(NewImage, OutputFileNam),

  io:fwrite(decode_string(NewImage)).
