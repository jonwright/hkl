#include "pseudoaxespinbutton.h"

PseudoAxeSpinButton::PseudoAxeSpinButton(Glib::ustring const & name, hkl::PseudoAxeInterface & pseudoAxe) :
  Gtk::Frame(),
  m_pseudoAxe(pseudoAxe),
  m_connected(true)
{
    // Set the name of the frame
    set_label(name);
    m_table = new Gtk::Table(4, 2);

    m_label_value = new Gtk::Label("xxx");
    m_label_min = new Gtk::Label("min : ");
    m_label_max = new Gtk::Label("max : ");

    // 1st spinbutton
    m_spinbutton_value = new Gtk::SpinButton();
    m_spinbutton_value->set_digits(3);
    m_spinbutton_value->set_numeric(true);
    m_spinbutton_value->set_increments(0.1, 1);
    m_spinbutton_value->set_update_policy(Gtk::UPDATE_IF_VALID);

    m_label_min_value = new Gtk::Label("xxx");
    m_label_max_value = new Gtk::Label("xxx");

    m_table->attach(*m_label_value, 0, 1, 0, 1, Gtk::FILL);
    m_table->attach(*m_spinbutton_value, 1, 2, 0, 1, Gtk::FILL);
    m_table->attach(*m_label_min, 0, 1, 1, 2, Gtk::FILL);
    m_table->attach(*m_label_max, 1, 2, 1, 2, Gtk::FILL);
    m_table->attach(*m_label_min_value, 0, 1, 2, 3, Gtk::FILL);
    m_table->attach(*m_label_max_value, 1, 2, 2, 3, Gtk::FILL);

    m_togglebutton = new Gtk::ToggleButton("xxx");
    m_table->attach(*m_togglebutton, 0, 2, 3, 4, Gtk::FILL);
    // add the table to the Frame
    add(*m_table);

    update();

    m_spinbutton_value->signal_value_changed().connect(mem_fun(*this, &PseudoAxeSpinButton::on_spinbutton_value_value_changed));
    m_togglebutton->signal_toggled().connect(mem_fun(*this, &PseudoAxeSpinButton::on_togglebutton_toggled));
}

PseudoAxeSpinButton::~PseudoAxeSpinButton(void)
{
  delete m_table;
  delete m_label_value;
  delete m_label_min;
  delete m_label_max;
  delete m_spinbutton_value;
  delete m_label_min_value;
  delete m_label_max_value;
  delete m_togglebutton;
}

void
PseudoAxeSpinButton::update(void)
{
    m_connected = false;
    char value_text[30];
    char min_value_text[30];
    char max_value_text[30];

    // update the toggleButton
    bool initialized = m_pseudoAxe.get_initialized();
    m_togglebutton->set_active(initialized);
    if (initialized)
        m_togglebutton->set_label("Uninitialize");
    else
        m_togglebutton->set_label("Initialize");

    // update m_spinbutton_value range;
    double min = m_pseudoAxe.get_min();
    double max = m_pseudoAxe.get_max();
    m_spinbutton_value->set_range(min, max);

    if (initialized)
      {
        // update min/max range and value.
        snprintf(min_value_text, 29, "%lf", min);
        snprintf(max_value_text, 29, "%lf", max);

        try
          {
            // update label
            double value = m_pseudoAxe.get_value();
            snprintf(value_text, 29, "%lf", value);
            m_spinbutton_value->set_value(value);
          }
        catch (HKLException &)
          {
            sprintf(value_text, "xxx");
          }
      }
    else
      {
        m_spinbutton_value->set_text("xxx");
        sprintf(value_text, "xxx");
        sprintf(min_value_text, "xxx");
        sprintf(max_value_text, "xxx");
      }
    bool writable = m_pseudoAxe.get_writable();
    m_spinbutton_value->set_sensitive(writable);

    m_label_value->set_text(value_text);
    m_label_min_value->set_text(min_value_text);
    m_label_max_value->set_text(max_value_text);
    m_connected = true;
}

// signal
PseudoAxeSpinButton::type_signal_value_changed
PseudoAxeSpinButton::signal_value_changed(void)
{
    return m_signal_value_changed;
}

// Callback
void
PseudoAxeSpinButton::on_spinbutton_value_value_changed(void)
{
    if (m_connected)
      {
        double value = m_spinbutton_value->get_value();
        try
          {
            m_pseudoAxe.set_value(value);
          }
        catch (HKLException &)
          {
          }
        update();

        m_signal_value_changed();
      }
}

void
PseudoAxeSpinButton::on_togglebutton_toggled(void)
{
    if (m_connected)
      {
        bool state = m_togglebutton->get_active();
        if (state)
          {
            try
              {
                m_pseudoAxe.initialize();
              }
            catch (HKLException &)
              {
              }
          }
        else
            m_pseudoAxe.uninitialize();
        update();
      }
}