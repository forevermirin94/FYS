//jquery: index melding
  $(function() {
    // run the currently selected effect
    function runEffect() {
      // get effect type from
      var selectedEffect = "blind"
 
      // run the effect
      $( "#effect" ).show( selectedEffect, 500, callback );
    };
 
    //callback function to bring a hidden box back
    function callback() {
      setTimeout(function() {
        $( "#effect:visible" ).removeAttr( "style" ).fadeOut();
      }, 6000 );
    };
 
    // set effect
    setTimeout(function() {
      runEffect();
    }, 500 );
 
    $( "#effect" ).hide();
  });